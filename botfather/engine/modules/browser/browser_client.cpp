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
		QString json(QString::fromStdString(message->GetArgumentList()->GetString(1).ToString()));
		QJsonDocument json_document(QJsonDocument::fromJson(json.toUtf8()));

		qDebug() << "Received code and json:" << callback_id << json;
		qDebug() << "Received string:" << QString::fromStdString(message->GetArgumentList()->GetString(2).ToString());
		qDebug() << "Created document:" << json_document;
		qDebug() << "Is object?" << json_document.isObject();

		QJsonValue root_value = json_document.object().value("ROOT");
		QVariant root_variant = root_value.toVariant();

		emit evalJavascriptResultReady(callback_id, root_variant);
		return true;
	}

	return false;
}
