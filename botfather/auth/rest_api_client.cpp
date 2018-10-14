#include "rest_api_client.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QUrlQuery>
#include <QCryptographicHash>

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
#include <QRandomGenerator>
#define GenerateRandomIntInRange(MIN, MAX) QRandomGenerator::global()->bounded(MIN, MAX);
#else
#define ImageSizeInBytes(qimage) static_cast<int>(qimage.sizeInBytes())
#define GenerateRandomIntInRange(MIN, MAX) qrand() % ((MAX + 1) - MIN) + MIN
#endif

RestApiClient::RestApiClient(QString software_slug, QString software_version, QString version_secret, QObject *parent)
	: QObject(parent)
	, software_slug(software_slug)
	, software_version(software_version)
	, version_secret(version_secret)
	, magic(generateMagic())
{
	network_manager = new QNetworkAccessManager(this);
	connect(network_manager, SIGNAL(finished(QNetworkReply*)), SLOT(networkReplyReceived(QNetworkReply*)));
}

QUrlQuery RestApiClient::preparePostData()
{
	QUrlQuery post_data;
	post_data.addQueryItem("software", software_slug);
	post_data.addQueryItem("version", software_version);
	post_data.addQueryItem("magic", magic);
	return post_data;
}

void RestApiClient::sendPostData(QUrlQuery post_data)
{
	QNetworkRequest network_request(getApiEndpoint());
	network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	network_manager->post(network_request, post_data.toString(QUrl::FullyEncoded).toUtf8());
}

void RestApiClient::networkReplyReceived(QNetworkReply *reply)
{
	// Check for network errors
	if (reply->error() != QNetworkReply::NoError){
		emit networkError(reply->error());
		reply->deleteLater();
		return;
	}	
	
	// Turn the replys plain text into json and free it's ressources.
	QByteArray response_data = reply->readAll();
	QJsonDocument json = QJsonDocument::fromJson(response_data);
	reply->deleteLater();
	
	QJsonArray errors = json.object().value("errors").toArray();
	if (!errors.isEmpty()){
		// => There are api related errors.
		emit restApiError(errors);
		return;
	}
	
	processJsonResponse(json);
}

QString RestApiClient::generateMagic() const
{
	const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	QString random_string;
	for (int i = 0; i < MAGIC_LENGTH; ++i) {
        int index = GenerateRandomIntInRange(0, possibleCharacters.length());
		QChar next_char = possibleCharacters.at(index);
		random_string.append(next_char);
	}
	return random_string;
}

bool RestApiClient::verifyHash(QString hashhex, QVector<QString> data_list) const
{
	QCryptographicHash hash(QCryptographicHash::Algorithm::Sha256);
	hash.addData(software_slug.toUtf8());
	hash.addData(software_version.toUtf8());
	hash.addData(version_secret.toUtf8());
	hash.addData(magic.toUtf8());
	for (QString data : data_list) {
		hash.addData(data.toUtf8());
	}
	return hash.result().toHex() == hashhex;
}
