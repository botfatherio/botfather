#include "browser_client.h"
#include <QDebug>
#include <QUrl>
#include <QUrlQuery>
#include <QRegularExpression>
#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include "browser_settings.h"

namespace {
    BrowserClient * g_instance = nullptr;
}

BrowserClient::BrowserClient()
{
	DCHECK(!g_instance);
	g_instance = this;
}

BrowserClient::~BrowserClient()
{
	g_instance = nullptr;
}

BrowserClient * BrowserClient::instance()
{
	return g_instance;
}

QImage BrowserClient::takeScreenshot()
{
    screenshot_lock.lockForRead();
    QImage copy = screenshot;
    screenshot_lock.unlock();

    return copy;
}

CefRefPtr<CefBrowser> BrowserClient::getBrowser() const
{
	return this->main_browser_;
}

void BrowserClient::closeAllBrowsers(bool force_close)
{
	// As this function is only called when we atually inted to close
	// the browser, allow the close process.
	this->allow_browser_to_close_ = true;

	// Anyway execute this function on the UI thread.
	if (!CefCurrentlyOn(TID_UI)) {
		CefPostTask(TID_UI, base::Bind(&BrowserClient::closeAllBrowsers, this, force_close));
		return;
	}

	// Request that the main browser close.
	if (this->main_browser_.get()) {
		this->main_browser_->GetHost()->CloseBrowser(force_close);
	}
}

void BrowserClient::blockResource(QString resource_pattern)
{
	modified_resources.append({resource_pattern, ""});
}

void BrowserClient::replaceResource(QString old_resource_pattern, QString new_resource_url)
{
	modified_resources.append({old_resource_pattern, new_resource_url});
}

void BrowserClient::unmodifyResource(QString resource_pattern)
{
	for (int i = 0; i < modified_resources.length(); i++) {
		QString pattern = modified_resources[i].first;
		if (pattern == resource_pattern) {
			modified_resources.remove(i);
			return;
		}
	}
}

void BrowserClient::unmodifyResources()
{
	this->modified_resources.clear();
}

bool BrowserClient::loading() const
{
	return this->is_loading;
}

void BrowserClient::setLoading(bool state)
{
	this->is_loading = state;
}

bool BrowserClient::DoClose(CefRefPtr<CefBrowser> browser)
{
	Q_UNUSED(browser)
	CEF_REQUIRE_UI_THREAD();

	// Closing the main window requires special handling. See the DoClose()
	// documentation in the CEF header for a detailed destription of this
	// process.

	// Only allow the browser to close if we wan't it to do so.
	if (this->allow_browser_to_close_) {
		// Allow the close
		return false;
	}

	// Don't allow the browser to close if we didn't allow it.
	// Also go to the dafault page and mark the browser as crashed.
	emit browserClosedSignal();
	return true;
}

void BrowserClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();
	this->main_browser_ = browser;
}

void BrowserClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	Q_UNUSED(browser)
	CEF_REQUIRE_UI_THREAD();

	// Free the browser pointer so that the browser can be destroyed.
	this->main_browser_ = nullptr;
}

bool BrowserClient::OnBeforePopup(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	const CefString& target_url,
	const CefString& target_frame_name,
	CefLifeSpanHandler::WindowOpenDisposition target_disposition,
	bool user_gesture,
	const CefPopupFeatures& popupFeatures,
	CefWindowInfo& windowInfo,
	CefRefPtr<CefClient>& client,
	CefBrowserSettings& settings,
	bool* no_javascript_access)
{
	Q_UNUSED(browser)
	Q_UNUSED(frame)
	Q_UNUSED(target_frame_name)
	Q_UNUSED(target_disposition)
	Q_UNUSED(user_gesture)
	Q_UNUSED(popupFeatures)
	Q_UNUSED(windowInfo)
	Q_UNUSED(client)
	Q_UNUSED(settings)
	Q_UNUSED(no_javascript_access)	
	
	// TODO: Eventually block certain popups from being opened at all.

	// Open popup url in osr browser.
	getBrowser()->GetMainFrame()->LoadURL(target_url);

	// And cancel the popup.
	return true;
}

void BrowserClient::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
	Q_UNUSED(browser)
	CEF_REQUIRE_UI_THREAD();

	QSettings settings;
	
	// Return true if the rectagle is provided.
	rect.Set(
		0,
		0,
		settings.value(browser::options::WIDTH, browser::fallback::WIDTH).toInt(),
		settings.value(browser::options::HEIGHT, browser::fallback::HEIGHT).toInt()
	);
}

void BrowserClient::OnPaint(
	CefRefPtr<CefBrowser> browser,
	PaintElementType type,
	const RectList& dirtyRects,
	const void* buffer,
	int width,
	int height)
{
	Q_UNUSED(browser)
	Q_UNUSED(type)
	Q_UNUSED(dirtyRects)
	CEF_REQUIRE_UI_THREAD();

	// QPixmap is stored on the XServer when using X11 backend, whereas QImage is just an "array in memory" of the client program.
	// So drawing QPixmaps on XWindow systems should be much faster than drawing QImages, as the data is already on the server, processed and ready to use.

	// QPixmap objects can be passed around by value since the QPixmap class uses implicit data sharing.
	// For more information, see the Implicit Data Sharing documentation.QPixmap objects can also be streamed.

    screenshot_lock.lockForWrite();
    screenshot = QImage(static_cast<const unsigned char*>(buffer), width, height, QImage::Format_RGB32).copy();
    screenshot_lock.unlock();

    screenshot_lock.lockForRead();
    emit paintSignal(screenshot);
    screenshot_lock.unlock();
}

void BrowserClient::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{
	Q_UNUSED(browser)
	Q_UNUSED(frame)
	Q_UNUSED(httpStatusCode)
	CEF_REQUIRE_UI_THREAD();
	emit this->loadingFinishedSignal();
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

	// Log load error message.
	//qDebug("Failed to load URL " << std::string(failedUrl) << " with error " << std::string(errorText) << " (" << errorCode << ").");
}

void BrowserClient::OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward)
{
	Q_UNUSED(browser)
	Q_UNUSED(canGoBack)
	Q_UNUSED(canGoForward)
	CEF_REQUIRE_UI_THREAD();
	this->is_loading = isLoading;
	emit loadingStateChangedSignal(isLoading);
}

void BrowserClient::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefLoadHandler::TransitionType transition_type)
{
	Q_UNUSED(browser)
	Q_UNUSED(frame)
	Q_UNUSED(transition_type)
	CEF_REQUIRE_UI_THREAD();
	emit this->loadingStartedSignal();
}

CefRequestHandler::ReturnValue BrowserClient::OnBeforeResourceLoad(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request,
	CefRefPtr<CefRequestCallback> callback)
{
	Q_UNUSED(browser)
	Q_UNUSED(frame)
	Q_UNUSED(callback)
	
	QString url = QString::fromStdString(request->GetURL().ToString());

	for (int i = 0; i < modified_resources.length(); i++) {
		
		QString pattern = modified_resources[i].first;
		QRegularExpression regex(pattern);
		
		if (!regex.match(url).hasMatch()) {
			continue;
		}
		
		QString new_url = modified_resources[i].second;
		if (new_url.isEmpty()) {
			qDebug() << "blocking" << url;
			return RV_CANCEL; // Block the resource from loading.
		}
		
		qDebug() << "replacing" << url << "with" << new_url;
		request->SetURL(new_url.toStdString());
	}

	return RV_CONTINUE;
}

void BrowserClient::OnPluginCrashed(CefRefPtr<CefBrowser> browser, const CefString& plugin_path)
{
	Q_UNUSED(browser);

	// Called on the browser process UI thread when a plugin has crashed.
	// |plugin_path| is the path of the plugin that crashed. 
	qDebug() << Q_FUNC_INFO << "Plugin Crashed:" << QString::fromStdString(std::string(plugin_path));
	emit pluginCrashedSignal();
}

void BrowserClient::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, CefRequestHandler::TerminationStatus status)
{
	Q_UNUSED(browser);

	// Called on the browser process UI thread when the render process terminates unexpectedly.
	// |status| indicates how the process terminated.

	switch (status) {
	case TS_ABNORMAL_TERMINATION:
		qDebug() << Q_FUNC_INFO << "Renderer Terminated. Non-zero exit status.";
		break;
	case TS_PROCESS_WAS_KILLED:
		qDebug() << Q_FUNC_INFO << "Renderer Was Killed. SIGKILL or task manager kill.";
		break;
	case TS_PROCESS_CRASHED:
		qDebug() << Q_FUNC_INFO << "Renderer Crashed. Segmentation fault.";
		break;
	default:
		qDebug() << "RIP " << status;
		break;
	}
	emit rendererCrashedSignal();
}
