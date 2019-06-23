#include "browser_client.hpp"
#include <QCborValue>
#include <QCborStreamReader>
#include <QDebug>


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
		int callback_id(message->GetArgumentList()->GetInt(0));
		QCborValue cbor_value;

		{
			CefRefPtr<CefBinaryValue> binary_value = message->GetArgumentList()->GetBinary(1);
			size_t buffer_size = binary_value->GetSize();

			char *buffer = new char[buffer_size];
			binary_value->GetData(buffer, buffer_size, 0);			

			QByteArray byte_array(buffer, buffer_size);
			QCborStreamReader cbor_stream_reader(byte_array);

			cbor_value = QCborValue::fromCbor(cbor_stream_reader);
			delete[] buffer;

			qDebug() << "Received casted cbor_value:" << cbor_value;
		}

		emit evalJavascriptResultReady(callback_id, cbor_value);
		return true;
	}

	return false;
}
