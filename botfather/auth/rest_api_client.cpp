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
		{ QCryptographicHash::Algorithm::Md5, "b37704ddfe0e5f63746898cb24a8872a" },
		{ QCryptographicHash::Algorithm::Sha1, "f91ec934af9972eea03b88cb778950354968d037" },
		{ QCryptographicHash::Algorithm::Sha224, "23c1be1c9cb20bdab713acd5231ea2fc76d7cbfab6e2b585eaa1d08a" },
		{ QCryptographicHash::Algorithm::Sha256, "fc2d7cc1f4d53578497915897bd1a481c6ef7f3c4925c7fbe1fc07c71ad7f973" },
		{ QCryptographicHash::Algorithm::Sha384, "4ff13cbe165e8af0dc62e377818d128033cb8c11e4273c81562604e2ca509647d1db07a188a97111123b8bf2d3b9d925" },
		{ QCryptographicHash::Algorithm::Sha512, "2d071509b7a6c53b327699dd4c97f0bc53ae3ae39d7c1858e9e3f312a99a6e46d4795a948eafe241668e7b640ce3d47e77bd532ed6a70ec89cd2e7a8dab4ba7f" }
	};
	return checksums.value(algorithm, QString());
}

void RestApiClient::replyReceived(QNetworkReply* reply)
{
	if (reply->error() != QNetworkReply::NoError){

		qDebug() << "Network reply error:" << reply->errorString();
		emit networkError(reply->error());
		reply->deleteLater();
		return;
	}

	if (!verifyReply(reply)) {
		qDebug() << "Network reply verification failed.";
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
			qDebug() << "Cert checksum missmatch:" << hash << checksum;
			return false;
		}
	}

	return true;
}
