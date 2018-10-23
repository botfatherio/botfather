#include "license_api_client.h"
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>

void LicenseApiClient::requestLicense(const QString &software, const QString &username, const QString &password)
{
	QUrlQuery post_data;
	post_data.addQueryItem("software", software);
	post_data.addQueryItem("username", username);
	post_data.addQueryItem("password", password);
	sendPostData(post_data);
}

QUrl LicenseApiClient::getApiEndpoint()
{
	return QUrl("https://botfather.io/api/v7/license/");
}

QString LicenseApiClient::certificateChecksum(QCryptographicHash::Algorithm algorithm)
{
	QMap<QCryptographicHash::Algorithm, QString> checksums = {
		{ QCryptographicHash::Algorithm::Md5, "35a2684532698f1e1ed984cfecd5469e" },
		{ QCryptographicHash::Algorithm::Sha1, "53f8734d33ea850bb38beb1f067d7aa5db9f2958" },
		{ QCryptographicHash::Algorithm::Sha224, "06a21f22039601c2b70f747c388ede0ed5b47307956acbd3a61ab417" },
		{ QCryptographicHash::Algorithm::Sha256, "8f2e29da866b3519fb292ad19a8cb1cf246257088abaa37635fd938f024a2144" },
		{ QCryptographicHash::Algorithm::Sha384, "4b06e4a16f5b901cb4570c9cba000b8baff0af1da9048a4df0b4291e91984f1d194582264f8227f9a95614b7fdad0ef8" },
		{ QCryptographicHash::Algorithm::Sha512, "b3e2c28f49ac13e87db23768fd5003f37ee07bf99fd042702f3b949f49a74624492981a3b0a9438fc7883a3ada5aa39013e4f8cdffeda2c381acaeee3e76c814" }
	};
	return checksums.value(algorithm, QString());
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
