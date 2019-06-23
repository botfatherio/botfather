#include "helper_app.hpp"
#include <QDataStream>
#include <QBuffer>
#include "bf_serializer.hpp"
#include "bf_debug_tools.hpp"

HelperApp::HelperApp()
{

}

bool HelperApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	if (message->GetName() == "eval_javascript")
	{
		CefRefPtr<CefV8Context> context = browser->GetMainFrame()->GetV8Context();
		context->Enter();

		CefRefPtr<CefV8Value> retval = CefRefPtr<CefV8Value>();
		CefRefPtr<CefV8Exception> exception = CefRefPtr<CefV8Exception>();

		int callback_id = message->GetArgumentList()->GetInt(0);
		CefString code = message->GetArgumentList()->GetString(1);
		CefString script_url = message->GetArgumentList()->GetString(2);
		int start_line = message->GetArgumentList()->GetInt(3);

		context->Eval(code, script_url, start_line, retval, exception);

		CefRefPtr<CefProcessMessage> result_msg = CefProcessMessage::Create("eval_javascript_result");
		CefRefPtr<CefListValue> result_msg_args = result_msg->GetArgumentList();

		result_msg_args->SetInt(0, callback_id);

		CefRefPtr<CefBinaryValue> binary_value = BFSerializer::CefV8ValueToCefBinaryValue(retval);
		result_msg_args->SetBinary(1, binary_value);

		// We must stay in context while parsing/accessing any V8 values, otherwise the renderer
		// process will crash with "Check failed: Currently not in a V8 context".
		context->Exit();

		browser->SendProcessMessage(PID_BROWSER, result_msg);
		return true;
	}

	return false;
}
