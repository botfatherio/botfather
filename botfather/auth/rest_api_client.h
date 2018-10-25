#ifndef BFP_AUTH_REST_API_CLIENT_H
#define BFP_AUTH_REST_API_CLIENT_H

#include <QVector>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>

class RestApiClient : public QObject
{
	Q_OBJECT
	
public:
	RestApiClient(QObject *parent = nullptr);
	virtual ~RestApiClient(){}

public slots:
	void replyReceived(QNetworkReply* reply);

signals:
	void networkError(QNetworkReply::NetworkError network_error);

protected:
	virtual void prepareRequest(QNetworkRequest &network_request) = 0;
	void sendPostData(QUrlQuery post_data);
	virtual QString certificateChecksum(QCryptographicHash::Algorithm) = 0;
	bool verifyReply(QNetworkReply* reply);
	virtual void processJsonResponse(QJsonDocument json) = 0;
	
private:
	QNetworkAccessManager *network_manager;
};

#endif // BFP_AUTH_REST_API_CLIENT_H
