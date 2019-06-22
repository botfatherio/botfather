#include "browser_client.hpp"
#include <QJsonDocument>
#include <QJsonObject>
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
	if (message->GetName() == "eval_javascript_result")
	{
		int callback_id(message->GetArgumentList()->GetInt(0));
		QVariant value;

		{
			CefRefPtr<CefBinaryValue> binary_value = message->GetArgumentList()->GetBinary(1);
			size_t buffer_size = binary_value->GetSize();

			char *buffer = new char[buffer_size];
			binary_value->GetData(buffer, buffer_size, 0);

			QByteArray byte_array(buffer, buffer_size);
			QDataStream data_stream(byte_array);

			QVariant variant;
			data_stream >> variant;

			value = variant;
			delete[] buffer;

			qDebug() << "Received casted variant:" << variant;
		}

		emit evalJavascriptResultReady(callback_id, value);
		return true;
	}

	return false;
}
