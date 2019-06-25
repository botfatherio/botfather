#include "helper_app.hpp"
#include <QDataStream>
#include <QBuffer>
#include "bf_converter.hpp"
#include "bf_debug_tools.hpp"

HelperApp::HelperApp()
{

}

bool HelperApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	/** Executes javascript in the browsers V8 context, serializes the result value and returns it to the requesting
	 ** browser process.
	 **
	 ** Expects:
	 ** - Index 0: (CefString) javascript code
	 ** - Index 1: (CefString) script url
	 ** - Index 2: (int) start line
	 **
	 ** Returns:
	 ** - Index 0: (bool) success
	 ** - Index 1: (CefBinaryValue(QCborValue)) return value
	 ** - Index 2: (CefBinaryValue(QVariant(QVariantMap)) exception
	 **/
	if (message->GetName() == "eval_javascript")
	{
		CefRefPtr<CefV8Context> context = browser->GetMainFrame()->GetV8Context();
		context->Enter();

		// Expected paramters
		CefString javascript_code = message->GetArgumentList()->GetString(1);
		CefString script_url = message->GetArgumentList()->GetString(2);
		int start_line = message->GetArgumentList()->GetInt(3);

		// Script evaluation results
		CefRefPtr<CefV8Exception> cef_v8_exception = CefRefPtr<CefV8Exception>();
		CefRefPtr<CefV8Value> cef_v8_result_value = CefRefPtr<CefV8Value>();
		bool success = context->Eval(javascript_code, script_url, start_line, cef_v8_result_value, cef_v8_exception);

		// Prepare the result process message
		CefRefPtr<CefProcessMessage> result_msg = CefProcessMessage::Create("eval_javascript_result");
		CefRefPtr<CefListValue> result_msg_args = result_msg->GetArgumentList();
		result_msg_args->SetBool(0, success);

		if (success)
		{
			QCborValue cbor_result = BFConverter::CefV8ValueToQCborValue(cef_v8_result_value);
			CefRefPtr<CefBinaryValue> binary_result = BFConverter::QCborValueToCefBinaryValue(cbor_result);
			result_msg_args->SetBinary(1, binary_result);
		}
		else
		{
			QVariantMap map;
			map["end_column"] = cef_v8_exception->GetEndColumn();
			map["end_position"] = cef_v8_exception->GetEndPosition();
			map["line_number"] = cef_v8_exception->GetLineNumber();
			map["message"] = QString::fromStdString(cef_v8_exception->GetMessage().ToString());
			map["script_resource_name"] = QString::fromStdString(cef_v8_exception->GetScriptResourceName().ToString());
			map["source_line"] = QString::fromStdString(cef_v8_exception->GetSourceLine().ToString());
			map["start_column"] = cef_v8_exception->GetStartColumn();
			map["start_position"] = cef_v8_exception->GetStartPosition();

			CefRefPtr<CefBinaryValue> binary_exception = BFConverter::QVariantToCefBinaryValue(map);
			result_msg_args->SetBinary(2, binary_exception);
		}

		// We must stay in context while parsing/accessing any V8 values, otherwise the renderer
		// process will crash with "Check failed: Currently not in a V8 context".
		context->Exit();

		browser->SendProcessMessage(PID_BROWSER, result_msg);
		return true;
	}

	return false;
}
