#include "bf_debug_tools.hpp"

void BFDebugTools::debug(CefRefPtr<CefBrowser> browser, const QString &message)
{
	CefRefPtr<CefProcessMessage> debug = CefProcessMessage::Create("print_debug_message");
	CefRefPtr<CefListValue> debug_args = debug->GetArgumentList();
	debug_args->SetString(0, CefString(message.toStdString()));
	browser->SendProcessMessage(PID_BROWSER, debug);
}
