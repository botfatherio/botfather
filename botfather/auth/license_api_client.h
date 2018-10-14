#ifndef BFP_AUTH_LICENSE_API_CLIENT_H
#define BFP_AUTH_LICENSE_API_CLIENT_H

#include "rest_api_client.h"

class LicenseApiClient : public RestApiClient
{
	Q_OBJECT
	
public:
	using RestApiClient::RestApiClient;
	
public slots:
	void requestStatus();
	
signals:
	void licenseReceived(int premend);
	
protected:
	QUrl getApiEndpoint();
	void processJsonResponse(QJsonDocument json);
};

#endif // BFP_AUTH_LICENSE_API_CLIENT_H