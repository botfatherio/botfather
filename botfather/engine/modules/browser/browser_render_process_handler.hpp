#ifndef BROWSER_RENDER_PROCESS_HANDLER_HPP
#define BROWSER_RENDER_PROCESS_HANDLER_HPP

#include "include/cef_render_process_handler.h"

class BrowserRenderProcessHandler : public CefRenderProcessHandler
{
public:
	BrowserRenderProcessHandler();

	bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override;

private:
	IMPLEMENT_REFCOUNTING(BrowserRenderProcessHandler)
};

#endif // BROWSER_RENDER_PROCESS_HANDLER_HPP
