#include "rest_api_client.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QUrlQuery>
#include <QCryptographicHash>

RestApiClient::RestApiClient(QObject *parent) : QObject(parent)
{
	network_manager = new QNetworkAccessManager(this);
	connect(network_manager, &QNetworkAccessManager::finished, this, &RestApiClient::replyReceived);
}

void RestApiClient::sendPostData(QUrlQuery post_data)
{
	QNetworkRequest network_request(getApiEndpoint());
	network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	network_manager->post(network_request, post_data.toString(QUrl::FullyEncoded).toUtf8());
}

void RestApiClient::replyReceived(QNetworkReply* reply)
{
	if (reply->error() != QNetworkReply::NoError){
		emit networkError(reply->error());
		reply->deleteLater();
		return;
	}

	if (!verifyReply(reply)) {
		emit networkError(QNetworkReply::SslHandshakeFailedError);
		reply->deleteLater();
		return;
	}

	QByteArray response_data = reply->readAll();
	QJsonDocument json = QJsonDocument::fromJson(response_data);
	reply->deleteLater();

	processJsonResponse(json);
}

bool RestApiClient::verifyReply(QNetworkReply* reply)
{
	QByteArray pem_cert = reply->sslConfiguration().peerCertificate().toPem();

	static const QVector<QCryptographicHash::Algorithm> algorithms = {
		QCryptographicHash::Md4,
		QCryptographicHash::Md5,
		QCryptographicHash::Sha1,
		QCryptographicHash::Sha224,
		QCryptographicHash::Sha256,
		QCryptographicHash::Sha384,
		QCryptographicHash::Sha512,
		QCryptographicHash::Sha3_224,
		QCryptographicHash::Sha3_256,
		QCryptographicHash::Sha3_384,
		QCryptographicHash::Sha3_512,
		QCryptographicHash::Keccak_224,
		QCryptographicHash::Keccak_256,
		QCryptographicHash::Keccak_384,
		QCryptographicHash::Keccak_512,
	};

	for (const QCryptographicHash::Algorithm &algorithm : algorithms)
	{
		QString checksum = certificateChecksum(algorithm);
		if (checksum.isEmpty())
		{
			continue;
		}

		QString hash = QCryptographicHash::hash(pem_cert, algorithm).toHex();
		if (hash != checksum)
		{
			return false;
		}
	}

	return true;
}
