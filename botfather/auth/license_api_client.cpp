#include "license_api_client.h"
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>

void LicenseApiClient::requestStatus()
{
	QUrlQuery post_data = preparePostData();
	sendPostData(post_data);
}

QUrl LicenseApiClient::getApiEndpoint()
{
	return QUrl("https://botfather.io/api/v6/status/");
}

void LicenseApiClient::processJsonResponse(QJsonDocument json)
{
	int curtime = json.object().value("curtime").toInt();
	bool premend = json.object().value("premend").toInt();
	QString hashhex = json.object().value("hash").toString();
	
	QVector<QString> data_list = {
		QString::number(curtime),
		QString::number(premend)
	};

	if (verifyHash(hashhex, data_list)){
		emit integrityError();
		return;
	}
	
	emit licenseReceived(curtime);
}