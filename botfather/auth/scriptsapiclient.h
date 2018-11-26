#ifndef SCRIPTSAPICLIENT_H
#define SCRIPTSAPICLIENT_H

#include "rest_api_client.h"
#include "../gui/scriptmanager/models/scriptrepository.h"

class ScriptsApiClient : public RestApiClient
{
	Q_OBJECT

public:
	using RestApiClient::RestApiClient;

public slots:
	void requestScripts();

signals:
	void scriptsReceived(const QVector<ScriptRepository::Data> &repo_data_list);
	void errorsReceived(const QJsonArray &error_codes);

protected:
	QUrl provideApiEndpoint() override;
	void processJsonResponse(QJsonDocument json) override;

private:
	ScriptRepository::Data jsonObjectToRepoData(const QJsonObject &object);
};

#endif // SCRIPTSAPICLIENT_H
