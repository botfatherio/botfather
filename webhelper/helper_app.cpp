#include "helper_app.hpp"
#include <iostream>
#include <string>
#include "bf_serializer.hpp"

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
		context->Exit();

		CefRefPtr<CefProcessMessage> result_msg = CefProcessMessage::Create("eval_javascript_result");
		CefRefPtr<CefListValue> result_msg_args = result_msg->GetArgumentList();

		QString serializedJson(BFSerializer::CefV8ValueToCompactJsonQString(retval));
		result_msg_args->SetInt(0, callback_id);
		result_msg_args->SetString(1, CefString(serializedJson.toStdString()));

		// FIXME: remove this. its for debugging
		result_msg_args->SetString(2, retval->GetStringValue());

		browser->SendProcessMessage(PID_BROWSER, result_msg);
		return true;
	}

	return false;
}
