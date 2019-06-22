#include "browser_render_process_handler.hpp"
#include <iostream>

BrowserRenderProcessHandler::BrowserRenderProcessHandler()
{

}

bool BrowserRenderProcessHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	CefRefPtr<CefV8Context> context = browser->GetMainFrame()->GetV8Context();
	CefRefPtr<CefV8Value> global = context->GetGlobal();
	CefRefPtr<CefV8Value> value = global->GetValue(CefString("document.title"));

	std::cout << value->GetStringValue().ToString() << std::endl << std::endl;
	return true;
}
