#include "browser_client.hpp"
#include <QCborStreamReader>
#include <QDebug>
#include "browser_util.hpp"

BrowserClient::BrowserClient(const QSize &size)
	: QObject()
	, BrowserLifeSpanHandler()
	, BrowserRenderHandler(this, size)
	, BrowserLoadHandler(this)
	, BrowserRequestHandler(this)
{

}

void BrowserClient::setScreenPointOffset(const QPoint &offset)
{
	BrowserRenderHandler::setScreenPointOffset(offset);
}

bool BrowserClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	if (message->GetName() == "print_debug_message")
	{
		qDebug() << "[Renderer Debug Message]" << QString::fromStdString(message->GetArgumentList()->GetString(0));
		return true;
	}

	if (message->GetName() == "eval_javascript_result")
	{
		QString result_id = QString::fromStdString(message->GetArgumentList()->GetString(0).ToString());
		bool success = message->GetArgumentList()->GetBool(1);

		// Not initialising these caused segmentation faults in slots receiving the evalJavascriptResultReady signal.
		// When they copied the uninitialised values they caused the application to crash from time to time.
		QCborValue result = QCborValue();
		QVariantMap exception = QVariantMap();

		if (success)
		{
			CefRefPtr<CefBinaryValue> binary_result = message->GetArgumentList()->GetBinary(2);
			QCborStreamReader cbor_stream_reader(BrowserUtil::convertCefBinaryValueToQByteArray(binary_result));

			result = QCborValue::fromCbor(cbor_stream_reader);
			qDebug() << "Received casted result:" << result;
		}
		else
		{
			CefRefPtr<CefBinaryValue> binary_exception = message->GetArgumentList()->GetBinary(3);
			QDataStream data_stream(BrowserUtil::convertCefBinaryValueToQByteArray(binary_exception));

			QVariant variant;
			data_stream >> variant;

			exception = variant.toMap();
			qDebug() << "Received casted exception:" << exception;
		}

		emit evalJavascriptResultReady(result_id, success, result, exception);
		return true;
	}

	return false;
}
