#include "browser_client.h"
#include <QDebug>
#include <QUrl>
#include <QUrlQuery>
#include <QSettings>
#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include "../shared/constants.h"

namespace {
	BrowserClient * g_instance = nullptr;
} // !namespace

BrowserClient::BrowserClient()
{
	DCHECK(!g_instance);
	g_instance = this;
}

BrowserClient::~BrowserClient()
{
	g_instance = nullptr;
}

// static
BrowserClient * BrowserClient::instance()
{
	return g_instance;
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

void BrowserClient::blockRessource(QString ressource_url)
{
	this->modified_ressources.insert(ressource_url, "");
}

void BrowserClient::replaceRessource(QString old_ressource_url, QString new_ressource_url)
{
	this->modified_ressources.insert(old_ressource_url, new_ressource_url);
}

void BrowserClient::unmodifyRessource(QString ressource_url)
{
	this->modified_ressources.remove(ressource_url);
}

void BrowserClient::unmodifyRessources()
{
	this->modified_ressources.clear();
}

bool BrowserClient::DoClose(CefRefPtr<CefBrowser> browser)
{
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
	// TODO: Eventually block certain popups from being opened at all.

	// Open popup url in osr browser.
	getBrowser()->GetMainFrame()->LoadURL(target_url);

	// And cancel the popup.
	return true;
}

bool BrowserClient::GetViewRect(
	CefRefPtr<CefBrowser> browser,
	CefRect& rect)
{
	CEF_REQUIRE_UI_THREAD();

	QSettings settings;
	
	// Return true if the rectagle is provided.
	rect.Set(
		0,
		0,
		settings.value("BROWSER_WIDTH", constants::BROWSER_WIDTH).toInt(),
		settings.value("BROWSER_HEIGHT", constants::BROWSER_HEIGHT).toInt()
	);
	return true;
}

void BrowserClient::OnPaint(
	CefRefPtr<CefBrowser> browser,
	PaintElementType type,
	const RectList& dirtyRects,
	const void* buffer,
	int width,
	int height)
{
	CEF_REQUIRE_UI_THREAD();

	// QPixmap is stored on the XServer when using X11 backend, whereas QImage is just an "array in memory" of the client program.
	// So drawing QPixmaps on XWindow systems should be much faster than drawing QImages, as the data is already on the server, processed and ready to use.

	// QPixmap objects can be passed around by value since the QPixmap class uses implicit data sharing.
	// For more information, see the Implicit Data Sharing documentation.QPixmap objects can also be streamed.

	QImage browser_image((unsigned char*)buffer, width, height, QImage::Format_RGB32);
	emit paintSignal(browser_image);
}

void BrowserClient::OnLoadEnd(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	int httpStatusCode)
{
	CEF_REQUIRE_UI_THREAD();
}

void BrowserClient::OnLoadError(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	ErrorCode errorCode,
	const CefString& errorText,
	const CefString& failedUrl)
{
	CEF_REQUIRE_UI_THREAD();

	// Don't display an error for downloaded files.
	if (errorCode == ERR_ABORTED) return;

	// Don't display an error for external protocols (eg. ts3server://).
	if (errorCode == ERR_UNKNOWN_URL_SCHEME) return;

	// Log load error message.
	//qDebug("Failed to load URL " << std::string(failedUrl) << " with error " << std::string(errorText) << " (" << errorCode << ").");
}

void BrowserClient::OnLoadingStateChange(
	CefRefPtr<CefBrowser> browser,
	bool isLoading,
	bool canGoBack,
	bool canGoForward)
{
	CEF_REQUIRE_UI_THREAD();
	Q_UNUSED(browser);
	Q_UNUSED(canGoBack);
	Q_UNUSED(canGoForward);

	// Emit overall bowser load status.
	emit loadingStateSignal(isLoading);
}

void BrowserClient::OnLoadStart(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefLoadHandler::TransitionType transition_type)
{
	CEF_REQUIRE_UI_THREAD();
	Q_UNUSED(browser);
	Q_UNUSED(frame);
	Q_UNUSED(transition_type);
}

CefRequestHandler::ReturnValue BrowserClient::OnBeforeResourceLoad(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request,
	CefRefPtr<CefRequestCallback> callback)
{
	Q_UNUSED(browser);
	Q_UNUSED(frame);
	Q_UNUSED(callback);
	
	QString url = QString::fromStdString(request->GetURL().ToString());
	
	if (!this->modified_ressources.contains(url)) {
		qDebug() << "not blocking" << url;
		return RV_CONTINUE;
	}
	
	QString new_url = this->modified_ressources.value(url);
	
	if (new_url.isEmpty()) {
		qDebug() << "blocking" << url;
		return RV_CANCEL; // Block the ressource from loading.
	}
	
	qDebug() << "replacing" << url << "with" << new_url;
	request->SetURL(new_url.toStdString());
	return RV_CONTINUE;
}

void BrowserClient::OnPluginCrashed(
	CefRefPtr<CefBrowser> browser,
	const CefString& plugin_path)
{
	Q_UNUSED(browser);

	// Called on the browser process UI thread when a plugin has crashed.
	// |plugin_path| is the path of the plugin that crashed. 
	qDebug() << Q_FUNC_INFO << "Plugin Crashed:" << QString::fromStdString(std::string(plugin_path));
	emit pluginCrashedSignal();
}

void BrowserClient::OnRenderProcessTerminated(
	CefRefPtr<CefBrowser> browser,
	CefRequestHandler::TerminationStatus status)
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
		//qDebug("RIP " << status);
		break;
	}
	emit rendererCrashedSignal();
}