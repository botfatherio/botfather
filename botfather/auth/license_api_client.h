#ifndef BFP_AUTH_LICENSE_API_CLIENT_H
#define BFP_AUTH_LICENSE_API_CLIENT_H

#include "rest_api_client.h"

class LicenseApiClient : public RestApiClient
{
	Q_OBJECT
	
public:
	using RestApiClient::RestApiClient;
	
public slots:
	void requestLicense(const QString &software, const QString &username, const QString &password);
	
signals:
	void licenseReceived(int curtime, int premend);
	void errorsReceived(QJsonArray error_codes);

protected:
	QUrl getApiEndpoint();
	QString certificateChecksum(QCryptographicHash::Algorithm algorithm);
	void processJsonResponse(QJsonDocument json);
};

#endif // BFP_AUTH_LICENSE_API_CLIENT_H
