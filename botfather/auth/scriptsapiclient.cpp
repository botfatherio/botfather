#include "scriptsapiclient.h"
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

void ScriptsApiClient::requestScripts()
{
	QUrlQuery post_data;
	sendPostData(post_data);
}

QUrl ScriptsApiClient::provideApiEndpoint()
{
	return QUrl("https://botfather.io/api/v2/scripts/");
}

void ScriptsApiClient::processJsonResponse(QJsonDocument json)
{
	if (!json.object().contains("errors"))
	{
		emit networkError(QNetworkReply::UnknownContentError);
		return;
	}

	if (!json.object().value("errors").toArray().isEmpty())
	{
		emit errorsReceived(json.object().value("errors").toArray());
		return;
	}

	QVector<BotRepo::Data> repo_data_list;

	for (QJsonValue raw_repo : json.object().value("scripts").toArray())
	{
		if (!raw_repo.isObject())
		{
			qDebug() << "Raw repo is not an object";
			continue;
		}

		QJsonObject raw_repo_object = raw_repo.toObject();
		BotRepo::Data repo_data = jsonObjectToRepoData(raw_repo_object);

		if (repo_data.isEmpty())
		{
			qDebug() << "Repo data is empty";
			continue;
		}

		repo_data_list.append(repo_data);
	}

	emit scriptsReceived(repo_data_list);
	emit finished();
}

BotRepo::Data ScriptsApiClient::jsonObjectToRepoData(const QJsonObject &object)
{
	return BotRepo::Data(
		object.value("name").toString(),
		object.value("developer").toString(),
		object.value("description").toString(),
		"", // local path
		object.value("git_clone_url").toString()
	);
}
