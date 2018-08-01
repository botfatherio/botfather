#include "status_api_client.h"
#include "rest_api_client.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

void StatusApiClient::requestStatus()
{
	QUrlQuery post_data = preparePostData();
	sendPostData(post_data);
}

QUrl StatusApiClient::getApiEndpoint()
{
	return QUrl("https://botfather.io/api/v6/status/");
}

void StatusApiClient::processJsonResponse(QJsonDocument json)
{
	int curtime = json.object().value("curtime").toInt();
	bool supported = json.object().value("supported").toBool();
	bool stable = json.object().value("stable").toBool();
	QString latest_stable_version = json.object().value("latest_stable_version").toString();
	QString hashhex = json.object().value("hash").toString();
	
	QList<QString> data_list = {
		QString::number(curtime),
		QString::number(static_cast<int>(supported)),
		QString::number(static_cast<int>(stable)),
		latest_stable_version
	};

	if (verifyHash(hashhex, data_list)){
		emit integrityError();
		return;
	}
	
	emit statusReceived(supported, stable, latest_stable_version);
}