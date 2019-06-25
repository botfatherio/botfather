#include "browser.hpp"
#include <QElapsedTimer>
#include <QTimer>
#include <QThread>
#include <QDebug>
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"
#include "../adapters/cef_key_event_adapter.hpp"
#include "../browser_settings.hpp"
#include "../../common/bf_key_mapper.hpp"
#include "../../common/bf_keymap.hpp"

Browser::Browser(const QString &group, const QString &id, CefRefPtr<CefBrowser> cef_browser)
	: m_group(group)
	, m_name(id)
	, m_cef_browser(cef_browser)
{
	CefRefPtr<CefClient> cef_client = m_cef_browser->GetHost()->GetClient();
	m_browser_client = static_cast<BrowserClient*>(cef_client.get());
	Q_ASSERT(m_browser_client);
}

CefRefPtr<CefBrowser> Browser::cefBrowser() const
{
	return m_cef_browser;
}

BrowserClient* Browser::client() const
{
	return m_browser_client;
}

QString Browser::group() const
{
	return m_group;
}

QString Browser::name() const
{
	return m_name;
}

QSize Browser::size() const
{
	return m_browser_client->size();
}

QRect Browser::rect() const
{
	return QRect(QPoint(0, 0), size());
}

QUrl Browser::url() const
{
	return QString::fromStdString(m_cef_browser->GetMainFrame()->GetURL());
}

QImage Browser::takeScreenshot()
{
	return m_browser_client->takeScreenshot();
}

bool Browser::isLoading() const
{
	return m_cef_browser->IsLoading();
}

bool Browser::finishLoading(int timeout_in_seconds)
{
	QElapsedTimer timer;
	timer.start();

	while (isLoading())
	{
		if (timer.hasExpired(timeout_in_seconds * 1000))
		{
			return false;
		}
		QThread::sleep(1);
	}

	return true;
}

bool Browser::canGoBack() const
{
	return m_cef_browser->CanGoBack();
}

bool Browser::canGoForward() const
{
	return m_cef_browser->CanGoForward();
}

void Browser::blockResource(const QString &resource)
{
	m_browser_client->blockResource(resource);
}

void Browser::replaceResource(const QString &resource_pattern, const QString &replacement_url)
{
	m_browser_client->replaceResource(resource_pattern, replacement_url);
}

void Browser::unmodifyResource(const QString &resource_pattern)
{
	m_browser_client->unmodifyResource(resource_pattern);
}

void Browser::unmodifyResources()
{
	m_browser_client->unmodifyResources();
}

void Browser::loadUrl(const QString &url)
{
	m_cef_browser->GetMainFrame()->LoadURL(url.toStdString());
}

void Browser::beOnUrl(const QString &the_url)
{
	if (QUrl(the_url) != url())
	{
		loadUrl(the_url);
	}
}

void Browser::goHome()
{
	loadUrl("about:version");
}

void Browser::reload()
{
	m_cef_browser->Reload();
}

void Browser::reloadIgnoringCache()
{
	m_cef_browser->ReloadIgnoreCache();
}

void Browser::stopLoading()
{
	m_cef_browser->StopLoad();
}

void Browser::goBack()
{
	m_cef_browser->GoBack();
}

void Browser::goForward()
{
	m_cef_browser->GoForward();
}

void Browser::executeJavascript(const QString &javascript_code)
{
	m_cef_browser->GetMainFrame()->ExecuteJavaScript(
		javascript_code.toStdString(),
		m_cef_browser->GetMainFrame()->GetURL(),
		0
	);
}

static void send_eval_javascript_message(CefRefPtr<CefBrowser> cef_browser, const QString &javascript_code)
{
	CefRefPtr<CefProcessMessage> msg= CefProcessMessage::Create("eval_javascript");
	CefRefPtr<CefListValue> args = msg->GetArgumentList();
	args->SetString(1, CefString(javascript_code.toStdString()));
	args->SetString(2, CefString("test.js"));
	cef_browser->SendProcessMessage(PID_RENDERER, msg);
}

bool Browser::evaluateJavascript(const QString &javascript_code, int timeout_in_ms, QCborValue &result, QVariantMap &exception, bool &timed_out)
{
	bool success = false;

	// Directly populating the reentrant |result| and |exception| caused crashes from time to time in
	// the QCborValue == operator. Using QMutex around all usages didn't help, neither did having a local
	// version of the result. Using local pointer seems to work.
	QCborValue *local_result = nullptr;
	QVariantMap *local_exception = nullptr;

	QEventLoop loop;
	QTimer timer;

	timer.setSingleShot(true);
	connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);

	connect(m_browser_client, &BrowserClient::evalJavascriptResultReady, &timer, &QTimer::stop);
	connect(m_browser_client, &BrowserClient::evalJavascriptResultReady, [this, &success, &local_result, &local_exception](bool s, const QCborValue &r, const QVariantMap &e) {
		success = s;
		if (s)
		{
			local_result = new QCborValue(r);
		}
		else
		{
			local_exception = new QVariantMap(e);
		}
	});

	// Calling QEventLoop::quit from within the lambda will occasionally cause crashes.
	// Connecting the signals in the given order works perfectly.
	connect(m_browser_client, &BrowserClient::evalJavascriptResultReady, &loop, &QEventLoop::quit);

	timer.start(timeout_in_ms);

	// CefBrowser::SendProcessMessage must be send from the main thread of the browser process.
	// TID_UI thread is the main thread in the browser process.
	CefPostTask(TID_UI, base::Bind(&send_eval_javascript_message, m_cef_browser, javascript_code));

	// Run the event loop (blocking)
	loop.exec();

	if (local_result)
	{
		result = *local_result;
		delete local_result;
	}
	if (local_exception)
	{
		exception = *local_exception;
		delete local_exception;
	}

	timed_out = !timer.isActive();
	return success;
}

static int convertToCefMouseButtonType(int qt_mouse_button)
{
	switch (qt_mouse_button) {
	case Qt::LeftButton:
		return MBT_LEFT;
	case Qt::MiddleButton:
		return MBT_MIDDLE;
	case Qt::RightButton:
		return MBT_RIGHT;
	default:
		return -1; // Unhandled mouse button type
	}
}

void Browser::pressMouse(const QPoint &position, int qt_mouse_button)
{
	holdMouse(position, qt_mouse_button);
	releaseMouse(position, qt_mouse_button);
}

void Browser::holdMouse(const QPoint &position, int qt_mouse_button)
{
	int button_code = convertToCefMouseButtonType(qt_mouse_button);
	if (button_code == -1) return;

	CefMouseEvent event;
	event.x = position.x();
	event.y = position.y();

	m_cef_browser->GetHost()->SendFocusEvent(true);
	m_cef_browser->GetHost()->SendMouseClickEvent(event, CefBrowserHost::MouseButtonType(button_code), false, 1);
}

void Browser::releaseMouse(const QPoint &position, int qt_mouse_button)
{
	int button_code = convertToCefMouseButtonType(qt_mouse_button);
	if (button_code == -1) return;

	CefMouseEvent event;
	event.x = position.x();
	event.y = position.y();

	m_cef_browser->GetHost()->SendMouseClickEvent(event, CefBrowserHost::MouseButtonType(button_code), true, 1);
}

void Browser::moveMouse(const QPoint &position)
{
	CefMouseEvent event;
	event.x = position.x();
	event.y = position.y();
	bool mouse_leaves = !rect().contains(position);
	m_cef_browser->GetHost()->SendMouseMoveEvent(event, mouse_leaves);
}

void Browser::scrollWheel(const QPoint &position, const QPoint &delta)
{
	CefMouseEvent event;
	event.x = position.x();
	event.y = position.y();
	m_cef_browser->GetHost()->SendMouseWheelEvent(event, delta.x(), delta.y());
}

bool Browser::canPressKey(const QString &bf_keycode) const
{
	// CefKeyEvents can be easily created from single characters. Though they might get interpreted wrong.
	return bf_keycode.length() == 1 || BF_KEYMAP.contains(bf_keycode);
}

bool Browser::canEnterText(const QString &text) const
{
	for (int i = 0; i < text.length(); ++i)
	{
		if (!canPressKey(text.at(i)))
		{
			return false;
		}
	}
	return true;
}

void Browser::pressKey(const QString &bf_keycode)
{
	holdKey(bf_keycode);
	releaseKey(bf_keycode);
}

void Browser::holdKey(const QString &bf_keycode)
{
	Qt::KeyboardModifier modifier = BFKeyMapper::mapBFKeycodeToQtKeyboardModifier(bf_keycode);
	if (!m_unreleased_keyboard_modifiers.testFlag(modifier))
	{
		m_unreleased_keyboard_modifiers = m_unreleased_keyboard_modifiers.setFlag(modifier, true);
	}

	CefKeyEventAdapter key_event(bf_keycode, m_unreleased_keyboard_modifiers);
	holdKey(key_event);
}

void Browser::releaseKey(const QString &bf_keycode)
{
	Qt::KeyboardModifier modifier = BFKeyMapper::mapBFKeycodeToQtKeyboardModifier(bf_keycode);
	if (m_unreleased_keyboard_modifiers.testFlag(modifier))
	{
		m_unreleased_keyboard_modifiers = m_unreleased_keyboard_modifiers.setFlag(modifier, false);
	}

	CefKeyEventAdapter key_event(bf_keycode, m_unreleased_keyboard_modifiers);
	releaseKey(key_event);
}

void Browser::holdKey(const CefKeyEvent &event)
{
	CefKeyEvent event_copy(event);
	event_copy.type = KEYEVENT_RAWKEYDOWN;
	m_cef_browser->GetHost()->SendKeyEvent(event_copy);
	event_copy.type = KEYEVENT_CHAR;
	m_cef_browser->GetHost()->SendKeyEvent(event_copy);
}

void Browser::releaseKey(const CefKeyEvent &event)
{
	CefKeyEvent event_copy(event);
	event_copy.type = KEYEVENT_KEYUP;
	m_cef_browser->GetHost()->SendKeyEvent(event_copy);
}

void Browser::enterText(const QString &text)
{
	for (int i = 0; i < text.length(); ++i)
	{
		pressKey(text.at(i));
	}
}

void Browser::holdKey(const QKeyEvent *event)
{
	if (event->matches(QKeySequence::Copy))
	{
		m_cef_browser->GetFocusedFrame()->Copy();
	}
	else if (event->matches(QKeySequence::Cut))
	{
		m_cef_browser->GetFocusedFrame()->Cut();
	}
	else if (event->matches(QKeySequence::Paste))
	{
		m_cef_browser->GetFocusedFrame()->Paste();
	}
	else if (event->matches(QKeySequence::Undo))
	{
		m_cef_browser->GetFocusedFrame()->Undo();
	}
	else if (event->matches(QKeySequence::Redo))
	{
		m_cef_browser->GetFocusedFrame()->Redo();
	}
	else if (event->matches(QKeySequence::SelectAll))
	{
		m_cef_browser->GetFocusedFrame()->SelectAll();
	}
	else
	{
		CefKeyEventAdapter key_event(event);
		holdKey(key_event);
	}
}

void Browser::releaseKey(const QKeyEvent *event)
{
	CefKeyEventAdapter key_event(event);
	releaseKey(key_event);
}
