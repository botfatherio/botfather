#include "rest_api_client.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QUrlQuery>
#include <QCryptographicHash>
#include <QFile>

RestApiClient::RestApiClient(QObject *parent) : QObject(parent)
{
	network_manager = new QNetworkAccessManager(this);
	connect(network_manager, &QNetworkAccessManager::finished, this, &RestApiClient::replyReceived);
}

void RestApiClient::sendPostData(QUrlQuery post_data)
{
	QNetworkRequest network_request;
	prepareRequest(network_request);
	network_manager->post(network_request, post_data.toString(QUrl::FullyEncoded).toUtf8());
}

void RestApiClient::prepareRequest(QNetworkRequest &network_request)
{
	QSslConfiguration ssl_config = QSslConfiguration::defaultConfiguration();
	adjustSslConfiguration(ssl_config);

	network_request.setSslConfiguration(ssl_config);
	network_request.setUrl(provideApiEndpoint());
	network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
}

void RestApiClient::adjustSslConfiguration(QSslConfiguration &ssl_config)
{
	static const QStringList cert_file_paths({
		":/botfather/certs/fullchain3.pem",
		":/botfather/certs/letsencryptauthorityx3.pem",
		":/botfather/certs/letsencryptauthorityx4.pem",
		":/botfather/certs/isrgrootx1.pem",
	});

	// Only use certs we trust during the ssl handshake. This includes our own api fullchain cert
	// and lets encrypts self signed certs. https://letsencrypt.org/certificates/

	QList<QSslCertificate> trusted_cas;

	for (const QString &path : cert_file_paths)
	{
		QFile cert_file(path);
		cert_file.open(QIODevice::ReadOnly);
		QSslCertificate cert(&cert_file);
		trusted_cas << cert;
	}

	ssl_config.setCaCertificates(trusted_cas);
}

QString RestApiClient::certificateChecksum(QCryptographicHash::Algorithm algorithm)
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
