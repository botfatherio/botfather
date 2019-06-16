#ifndef BROWSERCLIENT_H
#define BROWSERCLIENT_H

#include <QObject>
#include "include/cef_client.h"
#include "browser_life_span_handler.hpp"
#include "browser_render_handler.hpp"
#include "browser_load_handler.hpp"
#include "browser_request_handler.hpp"

class BrowserClient
	: public QObject
	, public CefClient
	, public BrowserLifeSpanHandler
	, public BrowserRenderHandler
	, public BrowserLoadHandler
	, public BrowserRequestHandler
{
	Q_OBJECT

public:
	explicit BrowserClient(const QSize &size);
	CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE { return this;}
	CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE { return this;}
	CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this;}
	CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE { return this;}

public slots:
	void setScreenPointOffset(const QPoint &offset);

signals:
	void paintSignal(const QImage &browser_image);
	void pluginCrashedSignal();
	void rendererCrashedSignal();
	void browserClosedSignal();
	void loadingStateChanged(bool is_loading, bool can_go_back, bool can_go_forward);
	void finishedLoadingUrl(const QString &url);

private:
	IMPLEMENT_REFCOUNTING(BrowserClient)
};

#endif // BROWSERCLIENT_H
