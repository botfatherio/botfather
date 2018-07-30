#ifndef BFP_AUTH_REST_API_CLIENT_H
#define BFP_AUTH_REST_API_CLIENT_H

#include <QList>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>

class RestApiClient : public QObject
{
	Q_OBJECT
	
public:
	static const int MAGIC_LENGTH = 10;
	RestApiClient(QString software_slug, QString software_version, QString version_secret, QObject *parent = nullptr);
	virtual ~RestApiClient(){}
	
public slots:
	// Receives the network reply and handles network and response errors.
	void networkReplyReceived(QNetworkReply* reply);
	
signals:
	void networkError(QNetworkReply::NetworkError network_error);
	void restApiError(QJsonArray error_codes);
	
	// Emitted when the response data has been manipulated or was not send by our webservice
	void integrityError();
	
protected:
	// Returns the url of the api endpoint
	virtual QUrl getApiEndpoint() = 0;
	
	// Adds software, version and magic to the post data. Override to add more.
	QUrlQuery preparePostData();
	
	// Sends a post post request with the given post data to the api endpoint.
	void sendPostData(QUrlQuery post_data);
	
	// Extracts the data from the json document, verifies it's integrity and emits a signal containing the results.
	virtual void processJsonResponse(QJsonDocument json) = 0;
	
	// Generates a random string send with the request and send back by the server, so we know we received the answer to our just send request
	QString generateMagic() const;
	
	// Creates a hash from the data list and compares it with the hash received from the server.
	bool verifyHash(QString hashhex, QList<QString> data_list) const;
	
private:
	QString software_slug;
	QString software_version;
	QString version_secret;
	QString magic;
	QNetworkAccessManager *network_manager;
};

#endif // BFP_AUTH_REST_API_CLIENT_H