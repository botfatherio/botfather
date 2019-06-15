#ifndef SCRIPTSAPICLIENT_H
#define SCRIPTSAPICLIENT_H

#include "rest_api_client.hpp"
#include "../gui/models/botrepo.hpp"

class ScriptsApiClient : public RestApiClient
{
	Q_OBJECT

public:
	using RestApiClient::RestApiClient;

public slots:
	void requestScripts();

signals:
	void scriptsReceived(const QVector<BotRepo> &bot_repo_list);
	void errorsReceived(const QJsonArray &error_codes);

protected:
	QUrl provideApiEndpoint() override;
	void processJsonResponse(QJsonDocument json) override;

private:
	BotRepo jsonObjectToRepoData(const QJsonObject &object);
};

#endif // SCRIPTSAPICLIENT_H
