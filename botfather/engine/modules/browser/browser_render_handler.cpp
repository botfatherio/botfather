#include "browser_render_handler.hpp"
#include "browser_client.hpp"
#include "include/wrapper/cef_helpers.h"

BrowserRenderHandler::BrowserRenderHandler(BrowserClient *parent, const QSize &browser_size)
	: m_parent(parent)
	, m_size(browser_size)
{

}

void BrowserRenderHandler::setScreenPointOffset(const QPoint &offset)
{
	m_screen_point_offset = offset;
}

QSize BrowserRenderHandler::size() const
{
	return m_size;
}

QImage BrowserRenderHandler::takeScreenshot()
{
	m_screenshot_lock.lockForRead();
	QImage copy = m_screenshot;
	m_screenshot_lock.unlock();
	return copy;
}

bool BrowserRenderHandler::GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
	// If this method returns false the rectangle from GetViewRect will be used.
	return false;
}

bool BrowserRenderHandler::GetScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo &screen_info)
{
	// If the screen info rectangle is left empty the rectangle from GetViewRect will be used.
	return false;
}

bool BrowserRenderHandler::GetScreenPoint(CefRefPtr<CefBrowser> browser, int view_x, int view_y, int &screen_x, int &screen_y)
{
	if (m_screen_point_offset.isNull())
	{
		return false;
	}

	screen_x = view_x + m_screen_point_offset.x();
	screen_y = view_y + m_screen_point_offset.y();
	return true;
}

void BrowserRenderHandler::GetViewRect(CefRefPtr<CefBrowser>, CefRect& rect)
{
	rect.Set(m_screen_point_offset.x(), m_screen_point_offset.y(), m_size.width(), m_size.height());
}

void BrowserRenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirty_rects, const void *buffer, int width, int height)
{
	// QPixmap is stored on the XServer when using X11 backend, whereas QImage is just an "array in memory" of the client program.
	// So drawing QPixmaps on XWindow systems should be much faster than drawing QImages, as the data is already on the server, processed and ready to use.

	m_screenshot_lock.lockForWrite();
	m_screenshot = QImage(static_cast<const unsigned char*>(buffer), width, height, QImage::Format_RGB32).copy();
	m_screenshot_lock.unlock();

	m_screenshot_lock.lockForRead();
	emit m_parent->paintSignal(m_screenshot);
	m_screenshot_lock.unlock();
}
