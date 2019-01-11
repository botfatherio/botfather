#ifndef BFP_ENGINE_MODULES_BROWSER_BROWSER_CLIENT_H
#define BFP_ENGINE_MODULES_BROWSER_BROWSER_CLIENT_H

#include <QObject>
#include <QImage>
#include <QSettings>
#include <QMap>
#include <QReadWriteLock>

#include "include/cef_client.h"

class BrowserClient :
	public QObject,
	public CefClient,
	public CefLifeSpanHandler,
	public CefRenderHandler,
	public CefLoadHandler,
	public CefRequestHandler
{
	Q_OBJECT

public:
	// Used by the cef app to create the one and only instance of this class. Calling
	// this more than once causes an error.
	explicit BrowserClient();
	
	// Frees the singletons global pointer.
	~BrowserClient() OVERRIDE;
	
	// Disable the clone operation (because it's a singleton).
	BrowserClient(BrowserClient const&) = delete;
	
	// Disable the copy operation (because it's a singleton).
	void operator=(BrowserClient const&) = delete;

	// Returns the pointer to the singleton instance of this object.
	static BrowserClient* instance();
	
    QImage takeScreenshot();
	
	// Returns the cef browser object. There is only one browser because we're always
	// running in offscreen rendering mode.
	CefRefPtr<CefBrowser> getBrowser() const;
	
	// Causes all browser windows (yes there is only one) to close, so that cef can
	// be shut down (yes, thats an implication: cef only can be closed successfully
	// when all browsers have been closed).
	void closeAllBrowsers(bool force_close);
	
	// Makes the resource no longe been loaded by the browser.
	void blockResource(QString resource_pattern);
	
	// Causes the browser to load the new resource instead of the original one.
	void replaceResource(QString old_resource_pattern, QString new_url);
	
	// Removes any replace or block rule from a given resource.
	void unmodifyResource(QString resource_pattern);
	
	// Removes all replace and block rules for all resources.
	void unmodifyResources();
	
	// Synces way of telling whether the browser loads something or not.
	bool loading() const;
	
	// Call this any time you cause the browser to load a resource.
	void setLoading(bool state);

	// CefClient methods:
	CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE { return this;}
	CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE { return this;}
	CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this;}
	CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE { return this;}

	// CefLifeSpanHandler methods
	bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	bool OnBeforePopup(
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
		bool* no_javascript_access) OVERRIDE;

	// CefRenderHandler methods:
	void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE;
	void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) OVERRIDE;

	// CefLoadHandler methods
	void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) OVERRIDE;
	void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) OVERRIDE;
	void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) OVERRIDE;
	void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefLoadHandler::TransitionType transition_type) OVERRIDE;

	// CefRequestHandler methods
	CefRequestHandler::ReturnValue OnBeforeResourceLoad(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			CefRefPtr<CefRequestCallback> callback) OVERRIDE;
	void OnPluginCrashed(CefRefPtr<CefBrowser> browser, const CefString& plugin_path) OVERRIDE;
	void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, CefRequestHandler::TerminationStatus status) OVERRIDE;

signals:
	void paintSignal(QImage browser_image);
	void pluginCrashedSignal();
	void rendererCrashedSignal();
	void browserClosedSignal();
	void loadingStateChangedSignal(bool state);
	void loadingStartedSignal();
	void loadingFinishedSignal();

private:
	// Keeps reference to the single, offscreen rendered browser.
	CefRefPtr<CefBrowser> main_browser_;

	// Only if this is true DoClose allows the browser to close.
	bool allow_browser_to_close_ = false;

	// Holds a reference to the qsettings.
	QSettings settings;
	
	// Whether the browser is loading or not.
	bool is_loading = true;
	
	// Contains all redirected urls. The first item of a pair is a regex pattern matching
	// the original url.
	// The second item is either empty, meaning the url should not be loaded (is blocked),
	// or there is a second url provided, meaning the second url should be loaded instead.
	QVector<QPair<QString, QString>> modified_resources;

    QReadWriteLock screenshot_lock;
    QImage screenshot;
	
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(BrowserClient)
};

#endif // !BFP_ENGINE_MODULES_BROWSER_BROWSER_CLIENT_H
