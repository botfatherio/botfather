#ifndef BROWSER_RENDER_HANDLER_HPP
#define BROWSER_RENDER_HANDLER_HPP

#include <QReadWriteLock>
#include <QSize>
#include <QPoint>
#include <QImage>
#include "include/cef_client.h"

class BrowserClient;

class BrowserRenderHandler : public CefRenderHandler
{
public:
	// https://magpcss.org/ceforum/apidocs3/projects/(default)/CefRenderHandler.html
	explicit BrowserRenderHandler(BrowserClient *parent, const QSize &browser_size);

	// GetScreenPoint translates view coordinates to actual screen coordinates.
	// The BrowserClient does not know were its Browser is displayed and thus
	// can't provide the offset by itself.
	// This value need to be updated everytime the associated BrowserWindow gets
	// moved or becomes focused. (Multiple BrowserWindows can display the same Browser).
	void setScreenPointOffset(const QPoint &offset);

	QSize size() const;
	QImage takeScreenshot();

	// Called to retrieve the root window rectangle in screen coordinates.
	bool GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;

	// Called to allow the client to fill in the CefScreenInfo object with appropriate values.
	bool GetScreenInfo( CefRefPtr<CefBrowser> browser, CefScreenInfo &screen_info) override;

	// Called to allow the client to fill in the CefScreenInfo object with appropriate values.
	bool GetScreenPoint(CefRefPtr<CefBrowser> browser, int view_x, int view_y, int& screen_x, int& screen_y) override;

	// Called to retrieve the view rectangle which is relative to screen coordinates.
	void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;

	//  Called when an element should be painted.
	void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirty_rects, const void *buffer, int width, int height) override;

private:
	BrowserClient *m_parent;
	QPoint m_screen_point_offset;
	QSize m_size;
	QReadWriteLock m_screenshot_lock;
	QImage m_screenshot;
	IMPLEMENT_REFCOUNTING(BrowserRenderHandler)
};

#endif // BROWSER_RENDER_HANDLER_HPP
