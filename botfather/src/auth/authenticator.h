#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <QObject>
#include <QNetworkReply>
#include <QJsonArray>

class Authenticator : public QObject
{
	Q_OBJECT
public:
	explicit Authenticator(QString software_slug, QString version_string, QString version_secret, QObject *parent = nullptr);
	
public slots:
	void authenticate(QString username, QString password);
	void networkReplyReceived(QNetworkReply* reply);
	
signals:
	void networkError(QNetworkReply::NetworkError network_error);
	void remoteApiError(QJsonArray error_codes);
	void integrityError();
	void authenticated(int curtime, int premend, bool stable);
	
	void errorsOccured(QNetworkReply::NetworkError network_error, QJsonArray error_codes, bool integrity_error);
	
private:
	QString software_slug;
	QString version_string;
	QString version_secret;
	QString magic;
	QString username;
	QString password;
	
	QString getRandomString(int random_string_length) const;
	bool verifyHash(QString hashhex, int premend, int curtime) const;
};

#endif // AUTHENTICATOR_H