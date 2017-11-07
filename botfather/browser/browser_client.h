#ifndef BFP__BROWSER__BROWSER_CLIENT_H
#define BFP__BROWSER__BROWSER_CLIENT_H

#include <QObject>
#include <QImage>
#include <QSettings>
#include "include/cef_client.h"

class BrowserClient :
	public QObject,
	public CefClient,
	public CefLifeSpanHandler,
	public CefRenderHandler,
	public CefLoadHandler,
	public CefRequestHandler {

	Q_OBJECT

public:
	explicit BrowserClient();
	~BrowserClient();

	// Provide access to the single global instance of this object.
	static BrowserClient* GetInstance();

	// CefClient methods:
	
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
		return this;
	}
	
	virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE {
		return this;
	}
	
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE {
		return this;
	}
	
	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE {
		return this;
	}

	// CefLifeSpanHandler methods
	
	virtual bool DoClose(
		CefRefPtr<CefBrowser> browser
	) OVERRIDE;
	
	virtual void OnAfterCreated(
		CefRefPtr<CefBrowser> browser
	) OVERRIDE;

	virtual void OnBeforeClose(
		CefRefPtr<CefBrowser> browser
	) OVERRIDE;

	virtual bool OnBeforePopup(
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
		bool* no_javascript_access
	) OVERRIDE;

	// CefRenderHandler methods:
	
	virtual bool GetViewRect(
		CefRefPtr<CefBrowser> browser,
		CefRect& rect
	) OVERRIDE;
	
	virtual void OnPaint(
		CefRefPtr<CefBrowser> browser,
		PaintElementType type,
		const RectList& dirtyRects,
		const void* buffer,
		int width,
		int height
	) OVERRIDE;

	// CefLoadHandler methods
	
	virtual void OnLoadEnd(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int httpStatusCode
	) OVERRIDE;

	virtual void OnLoadError(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl
	) OVERRIDE;

	virtual void OnLoadingStateChange(
		CefRefPtr<CefBrowser> browser,
		bool isLoading,
		bool canGoBack,
		bool canGoForward
	) OVERRIDE;

	virtual void OnLoadStart(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefLoadHandler::TransitionType transition_type
	) OVERRIDE;

	// CefRequestHandler methods
	
	virtual CefRequestHandler::ReturnValue OnBeforeResourceLoad(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		CefRefPtr<CefRequestCallback> callback
	) OVERRIDE;

	virtual void OnPluginCrashed(
		CefRefPtr<CefBrowser> browser,
		const CefString& plugin_path
	) OVERRIDE;

	virtual void OnRenderProcessTerminated(
		CefRefPtr<CefBrowser> browser,
		CefRequestHandler::TerminationStatus status
	) OVERRIDE;

	// Custom methods
	
	CefRefPtr<CefBrowser> GetBrowser()
	{
		return main_browser_;
	}

	// Request that all existing browser windows close.
	void CloseAllBrowsers(bool force_close);

signals:
	void paintSignal(QImage browser_image);
	void pluginCrashedSignal();
	void rendererCrashedSignal();
	void browserClosedSignal();
	void loadingStateSignal(bool state);

private:
	// Keeps reference to the single, offscreen rendered browser.
	CefRefPtr<CefBrowser> main_browser_;

	// Only if this is true DoClose allows the browser to close.
	bool allow_browser_to_close_ = false;

	// Holds a reference to the qsettings.
	QSettings settings;

	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(BrowserClient);
};

#endif // !BFP__BROWSER__BROWSER_CLIENT_H