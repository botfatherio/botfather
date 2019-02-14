#ifndef BROWSERCLIENT_H
#define BROWSERCLIENT_H

#include <QObject>
#include <QUuid>
#include <QImage>
#include <QSettings>
#include <QMap>
#include <QReadWriteLock>
#include "include/cef_client.h"

class BrowserClient
	: public QObject
	, public CefClient
	, public CefLifeSpanHandler
	, public CefRenderHandler
	, public CefLoadHandler
	, public CefRequestHandler
{
	Q_OBJECT

public:
	explicit BrowserClient(const QString &id, const QSize &size);

	// Used to identify persistent browsers. Anonymous/non-persistent browsers have no name.
	QString id() const;

	// Returns the browser fixed size
	QSize size() const;

	// Browsers having a name are considered persistent.
	bool isPersistent() const;

	// Returns the browsers latest frame threadsafe
	QImage takeScreenshot();

	// Makes the resource no longe been loaded by the browser.
	void blockResource(const QString &resource_pattern);

	// Makes the browser replace any resource matching the 'resource_pattern' with 'replacement_url'.
	void replaceResource(const QString &resource_pattern, const QString &replacement_url);

	// Removes any replace or block rule from a given resource.
	void unmodifyResource(const QString &resource_pattern);

	// Removes all replace and block rules for all resources.
	void unmodifyResources();

	// CefClient methods
	CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE { return this;}
	CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE { return this;}
	CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this;}
	CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE { return this;}

	// CefLifeSpanHandler methods
	bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	bool OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, const CefString& target_frame_name, CefLifeSpanHandler::WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client, CefBrowserSettings& settings, bool* no_javascript_access) OVERRIDE;

	// CefRenderHandler methods:
	void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE;
	void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) OVERRIDE;
	//void OnAcceleratedPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirty_rects, void *share_handle) OVERRIDE; // FIXME: take a look at accelerated paint

	// CefLoadHandler methods
	void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) OVERRIDE;
	void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) OVERRIDE;
	//void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefLoadHandler::TransitionType transition_type) OVERRIDE;
	//void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) OVERRIDE;

	// CefRequestHandler methods
	CefRequestHandler::ReturnValue OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback) OVERRIDE;
	void OnPluginCrashed(CefRefPtr<CefBrowser> browser, const CefString& plugin_path) OVERRIDE;
	void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, CefRequestHandler::TerminationStatus status) OVERRIDE;

signals: // FIXME: rename those signals. and remote the signal suffix
	void paintSignal(QImage browser_image);
	void pluginCrashedSignal();
	void rendererCrashedSignal();
	void browserClosedSignal();
	void loadingStateChangedSignal(bool state);

private:
	QString m_id;
	QSize m_size;
	QReadWriteLock m_screenshot_lock;
	QImage m_screenshot;

	// Contains all redirected urls. The first item of a pair is a regex pattern matching
	// the original url.
	// The second item is either empty, meaning the url should not be loaded (is blocked),
	// or there is a second url provided, meaning the second url should be loaded instead.
	QVector<QPair<QString, QString>> m_modified_resources;

	IMPLEMENT_REFCOUNTING(BrowserClient)
};

#endif // BROWSERCLIENT_H
