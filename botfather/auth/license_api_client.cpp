#include "license_api_client.h"
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

void LicenseApiClient::requestLicense(const QString &software, const QString &username, const QString &password)
{
	QUrlQuery post_data;
	post_data.addQueryItem("software", software);
	post_data.addQueryItem("username", username);
	post_data.addQueryItem("password", password);
	sendPostData(post_data);
}

QUrl LicenseApiClient::provideApiEndpoint()
{
	return QUrl("https://botfather.io/api/v7/license/");
}

void LicenseApiClient::processJsonResponse(QJsonDocument json)
{
	if (!json.object().contains("errors"))
	{
		emit networkError(QNetworkReply::UnknownContentError);
		return;
	}

	if (!json.object().value("errors").toArray().isEmpty())
	{
		emit errorsReceived(json.object().value("errors").toArray());
		return;
	}

	if (!json.object().contains("curtime") || !json.object().contains("premend"))
	{
		emit networkError(QNetworkReply::UnknownContentError);
		return;
	}
	int curtime = json.object().value("curtime").toInt();
	int premend = json.object().value("premend").toInt();
	emit licenseReceived(curtime, premend);
}
