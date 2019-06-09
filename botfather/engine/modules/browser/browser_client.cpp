#include "browser_client.h"
#include <QUrl>
#include <QUrlQuery>
#include <QRegularExpression>
#include <QDebug>
#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include "browser_settings.h"

BrowserClient::BrowserClient(const QSize &size) : QObject(), m_size(size)
{

}

QSize BrowserClient::size() const
{
	return m_size;
}

QImage BrowserClient::takeScreenshot()
{
	m_screenshot_lock.lockForRead();
	QImage copy = m_screenshot;
	m_screenshot_lock.unlock();
	return copy;
}

void BrowserClient::blockResource(const QString &resource_pattern)
{
	m_modified_resources.append({resource_pattern, ""});
}

void BrowserClient::replaceResource(const QString &resource_pattern, const QString &replacement_url)
{
	m_modified_resources.append({resource_pattern, replacement_url});
}

void BrowserClient::unmodifyResource(const QString &resource_pattern)
{
	for (int i = 0; i < m_modified_resources.length(); i++)
	{
		QString pattern = m_modified_resources[i].first;
		if (pattern == resource_pattern)
		{
			m_modified_resources.remove(i);
			return;
		}
	}
}

void BrowserClient::unmodifyResources()
{
	m_modified_resources.clear();
}

bool BrowserClient::DoClose(CefRefPtr<CefBrowser> browser)
{
	Q_UNUSED(browser)
	CEF_REQUIRE_UI_THREAD();

	// Closing the main window requires special handling. See the DoClose()
	// documentation in the CEF header for a detailed destription of this
	// process.

	return false; // Allow the browser to close
}

void BrowserClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	Q_UNUSED(browser)
	CEF_REQUIRE_UI_THREAD();
}

void BrowserClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	Q_UNUSED(browser)
	CEF_REQUIRE_UI_THREAD();
}

bool BrowserClient::OnBeforePopup(CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, const CefString&, const CefString&, CefLifeSpanHandler::WindowOpenDisposition, bool, const CefPopupFeatures&, CefWindowInfo&, CefRefPtr<CefClient>&, CefBrowserSettings&, bool*)
{
	// TODO: We could provide the user with a list of anonymous browsers. The user then could loop through that list and pick the browser he wants.
	return true; // Prevent all popups from opening.
}

void BrowserClient::GetViewRect(CefRefPtr<CefBrowser>, CefRect& rect)
{
	CEF_REQUIRE_UI_THREAD();
	rect.Set(0, 0, m_size.width(), m_size.height());
}

void BrowserClient::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height)
{
	Q_UNUSED(browser)
	Q_UNUSED(type)
	Q_UNUSED(dirtyRects)
	CEF_REQUIRE_UI_THREAD();

	// QPixmap is stored on the XServer when using X11 backend, whereas QImage is just an "array in memory" of the client program.
	// So drawing QPixmaps on XWindow systems should be much faster than drawing QImages, as the data is already on the server, processed and ready to use.

	m_screenshot_lock.lockForWrite();
	m_screenshot = QImage(static_cast<const unsigned char*>(buffer), width, height, QImage::Format_RGB32).copy();
	m_screenshot_lock.unlock();

	m_screenshot_lock.lockForRead();
	emit paintSignal(m_screenshot);
	m_screenshot_lock.unlock();
}

void BrowserClient::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
{
	Q_UNUSED(browser)
	Q_UNUSED(frame)
	Q_UNUSED(errorText)
	Q_UNUSED(failedUrl)
	CEF_REQUIRE_UI_THREAD();

	// Don't display an error for downloaded files.
	if (errorCode == ERR_ABORTED) return;

	// Don't display an error for external protocols (eg. ts3server://).
	if (errorCode == ERR_UNKNOWN_URL_SCHEME) return;
}

void BrowserClient::OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward)
{
	Q_UNUSED(browser)
	CEF_REQUIRE_UI_THREAD();
	emit loadingStateChanged(isLoading, canGoBack, canGoForward);
}

void BrowserClient::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{
	Q_UNUSED(frame)
	Q_UNUSED(httpStatusCode)
	emit finishedLoadingUrl(QString::fromStdString(browser->GetMainFrame()->GetURL()));
}

CefRequestHandler::ReturnValue BrowserClient::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback)
{
	Q_UNUSED(browser)
	Q_UNUSED(frame)
	Q_UNUSED(callback)

	QString url = QString::fromStdString(request->GetURL().ToString());

	for (int i = 0; i < m_modified_resources.length(); i++)
	{
		QString pattern = m_modified_resources[i].first;
		QRegularExpression regex(pattern);

		if (!regex.match(url).hasMatch())
		{
			continue;
		}

		QString new_url = m_modified_resources[i].second;
		if (new_url.isEmpty())
		{
			qDebug() << "Blocking resource" << url;
			return RV_CANCEL; // Block the resource from loading.
		}

		qDebug() << "Replacing resource" << url << "with" << new_url;
		request->SetURL(new_url.toStdString());
	}

	return RV_CONTINUE;
}

void BrowserClient::OnPluginCrashed(CefRefPtr<CefBrowser> browser, const CefString& plugin_path)
{
	Q_UNUSED(browser);

	// Called on the browser process UI thread when a plugin has crashed.
	// |plugin_path| is the path of the plugin that crashed.
	qDebug() << "Plugin Crashed:" << QString::fromStdString(std::string(plugin_path));
	emit pluginCrashedSignal();
}

void BrowserClient::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, CefRequestHandler::TerminationStatus status)
{
	Q_UNUSED(browser);

	// Called on the browser process UI thread when the render process terminates unexpectedly.
	// |status| indicates how the process terminated.

	switch (status) {
	case TS_ABNORMAL_TERMINATION:
		qDebug() << "Renderer Terminated. Non-zero exit status.";
		break;
	case TS_PROCESS_WAS_KILLED:
		qDebug() << "Renderer Was Killed. SIGKILL or task manager kill.";
		break;
	case TS_PROCESS_CRASHED:
		qDebug() << "Renderer Crashed. Segmentation fault.";
		break;
	default:
		qDebug() << "Abnormal CEF termination happened." << status;
		break;
	}
	emit rendererCrashedSignal();
}
