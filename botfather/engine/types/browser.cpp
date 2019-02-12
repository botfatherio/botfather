#include "browser.h"
#include <QElapsedTimer>
#include <QThread>
#include <QDebug>
#include "../modules/browser/browser_settings.h"

Browser::Browser(const QString &group, const QString &name, const QSize &size)
{
	m_cef_browser = BrowserHost::instance()->createManagedBrowser(group, name, size);
	CefRefPtr<CefClient> cef_client = m_cef_browser->GetHost()->GetClient();
	m_browser_client = static_cast<BrowserClient*>(cef_client.get());
	Q_ASSERT(m_browser_client);
}

Browser::Browser(const QString &group, const QString &name)
	: Browser(group, name, browser::fallback::SIZE)
{

}

Browser::Browser(const QString &group, const QSize &size)
	: Browser(group, QString(), size)
{

}

Browser::Browser(const QString &group)
	: Browser(group, QString())
{

}

QImage Browser::takeScreenshot()
{
	return m_browser_client->takeScreenshot();
}

QSize Browser::size() const
{
	return m_browser_client->size();
}

QRect Browser::rect() const
{
	return QRect(QPoint(0, 0), size());
}

QString Browser::name() const
{
	return m_browser_client->name();
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

QUrl Browser::url() const
{
	return QString::fromStdString(m_cef_browser->GetMainFrame()->GetURL());
}

void Browser::reload()
{
	m_cef_browser->Reload();
}

void Browser::reloadIgnoringCache()
{
	m_cef_browser->ReloadIgnoreCache();
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

void Browser::stopLoading()
{
	m_cef_browser->StopLoad();
}

bool Browser::canGoBack() const
{
	return m_cef_browser->CanGoBack();
}

bool Browser::canGoForward() const
{
	return m_cef_browser->CanGoForward();
}

void Browser::goBack()
{
	m_cef_browser->GoBack();
}

void Browser::goForward()
{
	m_cef_browser->GoForward();
}

void Browser::pressMouse(const CefBrowserHost::MouseButtonType &button_type, const QPoint &position)
{
	CefMouseEvent event;
	event.x = position.x();
	event.y = position.y();
	m_cef_browser->GetHost()->SendFocusEvent(true);
	m_cef_browser->GetHost()->SendMouseClickEvent(event, button_type, false, 1);
}

void Browser::releaseMouse(const CefBrowserHost::MouseButtonType &button_type, const QPoint &position)
{
	CefMouseEvent event;
	event.x = position.x();
	event.y = position.y();
	m_cef_browser->GetHost()->SendMouseClickEvent(event, button_type, true, 1);
}

void Browser::moveMouse(const QPoint &position)
{
	CefMouseEvent event;
	event.x = position.x();
	event.y = position.y();
	bool mouse_leave = rect().contains(position);
	m_cef_browser->GetHost()->SendMouseMoveEvent(event, mouse_leave);
}

void Browser::scrollWheel(const QPoint &position, int delta_x, int delta_y)
{
	CefMouseEvent event;
	event.x = position.x();
	event.y = position.y();
	m_cef_browser->GetHost()->SendMouseWheelEvent(event, delta_x, delta_y);
}

void Browser::executeJavascript(const QString &javascript_code)
{
	m_cef_browser->GetMainFrame()->ExecuteJavaScript(
		javascript_code.toStdString(),
		m_cef_browser->GetMainFrame()->GetURL(),
		0
	);
}
