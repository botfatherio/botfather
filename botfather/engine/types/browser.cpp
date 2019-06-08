#include "browser.h"
#include <QElapsedTimer>
#include <QThread>
#include <QDebug>
#include "../modules/browser/browser_settings.h"

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
	bool mouse_leave = rect().contains(position);
	m_cef_browser->GetHost()->SendMouseMoveEvent(event, mouse_leave);
}

void Browser::scrollWheel(const QPoint &position, const QPoint &delta)
{
	CefMouseEvent event;
	event.x = position.x();
	event.y = position.y();
	m_cef_browser->GetHost()->SendMouseWheelEvent(event, delta.x(), delta.y());
}
