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

bool LicenseApiClient::isLicenseActive() const
{
	return m_premend > m_curtime;
}

bool LicenseApiClient::isBotRuntimeLimited() const
{
	return allowedBotRuntimeInSecs() > 0;
}

int LicenseApiClient::allowedBotRuntimeInSecs() const
{
	// 90 minutes for non premiums, unlimited (-1) for premiums
	return isLicenseActive() ? -1 : 90 * 60;
}

bool LicenseApiClient::isNumberOfRunningBotsLimited() const
{
	return maxNumberOfRunningBotsAllowed() > 0;
}

int LicenseApiClient::maxNumberOfRunningBotsAllowed() const
{
	// 1 for non premiums, unlimited (-1) for premiums
	return isLicenseActive() ? -1 : 1;
}

void LicenseApiClient::resetLicense()
{
	m_premend = 0;
	m_curtime = 1;
}

QUrl LicenseApiClient::provideApiEndpoint()
{
	return QUrl("https://botfather.io/api/v7/license/");
}

void LicenseApiClient::processJsonResponse(QJsonDocument json)
{
	if (!json.object().contains("errors"))
	{
		qDebug() << "contains no errors array";
		emit networkError(QNetworkReply::UnknownContentError);
		return;
	}

	if (!json.object().value("errors").toArray().isEmpty())
	{
		qDebug() << "contains errors";
		emit errorsReceived(json.object().value("errors").toArray());
		return;
	}

	if (!json.object().contains("curtime") || !json.object().contains("premend"))
	{
		qDebug() << "curtime or premend missing";
		emit networkError(QNetworkReply::UnknownContentError);
		return;
	}

	m_curtime = json.object().value("curtime").toInt();
	m_premend = json.object().value("premend").toInt();

	emit licenseReceived(m_curtime, m_premend);
	emit finished();
}
