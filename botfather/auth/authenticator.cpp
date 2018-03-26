#include "authenticator.h"
#include <QDebug>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>

Authenticator::Authenticator(QString software_slug, QString version_string, QString version_secret, QObject *parent) : QObject(parent)
{
	this->software_slug = software_slug;
	this->version_string = version_string;
	this->version_secret = version_secret;
}

void Authenticator::authenticate(QString username, QString password)
{
	this->username = username;
	this->password = password;
	
	// Generate a random string the webservice will use to send back an salted hash of it's answer.
	magic = getRandomString(10);

	// Prepare post date to be send to the webservice
	QUrlQuery post_data;
	post_data.addQueryItem("username", username);
	post_data.addQueryItem("password", password);
	post_data.addQueryItem("software", software_slug);
	post_data.addQueryItem("version", version_string);
	post_data.addQueryItem("magic", magic);
	
	// Setup network manager to handle replies aswell as errors.
	QNetworkAccessManager *network_manager = new QNetworkAccessManager(this);
	connect(network_manager, SIGNAL(finished(QNetworkReply*)), SLOT(networkReplyReceived(QNetworkReply*)));
	
	// Send the request
	QNetworkRequest network_request(QUrl("https://botfather.io/api/v3/auth/"));
	network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	network_manager->post(network_request, post_data.toString(QUrl::FullyEncoded).toUtf8());
}

void Authenticator::networkReplyReceived(QNetworkReply *reply)
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
		emit remoteApiError(errors);
		return;
	}
	
	int curtime = json.object().value("curtime").toInt();
	int premend = json.object().value("premend").toInt();
	bool stable = json.object().value("stable").toBool();
	QString hashhex = json.object().value("hash").toString();
	
	if (!this->verifyHash(hashhex, premend, curtime)){
		// => This reply has been manipulated and was not send by our webservice.
		emit integrityError();
		return;
	}
	
	// The user is permitted to use the software.
	emit this->authenticated(curtime, premend, stable);
}

QString Authenticator::getRandomString(int random_string_length) const
{
	const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	QString random_string;
	for (int i=0; i<random_string_length; ++i) {
		int index = qrand() % possibleCharacters.length();
		QChar next_char = possibleCharacters.at(index);
		random_string.append(next_char);
	}
	return random_string;
}

bool Authenticator::verifyHash(QString hashhex, int premend, int curtime) const
{
	QCryptographicHash hash(QCryptographicHash::Algorithm::Sha256);
	hash.addData(QString::number(premend).toUtf8());
	hash.addData(QString::number(curtime).toUtf8());
	hash.addData(this->magic.toUtf8());
	hash.addData(this->username.toUtf8());
	hash.addData(this->password.toUtf8());
	hash.addData(this->version_secret.toUtf8());
	return hash.result().toHex() == hashhex;
}