#include "browser.h"
#include <QElapsedTimer>
#include <QThread>
#include <QDebug>
#include "../modules/browser/browser_settings.h"
#include "../modules/browser/adapters/cef_key_event_adapter.h"
#include "../modules/common/bf_key_mapper.h"
#include "../modules/common/bf_keymap.h"

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
