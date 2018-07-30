#ifndef BFP_AUTH_STATUS_API_CLIENT_H
#define BFP_AUTH_STATUS_API_CLIENT_H

#include "rest_api_client.h"

class StatusApiClient : public RestApiClient
{
	Q_OBJECT
	
public:
	using RestApiClient::RestApiClient;
	
public slots:
	void requestStatus();
	
signals:
	void statusReceived(bool supported, bool stable, QString latest_stable_version);
	
protected:
	QUrl getApiEndpoint();
	void processJsonResponse(QJsonDocument json);
};

#endif // BFP_AUTH_STATUS_API_CLIENT_H