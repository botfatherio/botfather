#include "scriptsapiclient.hpp"
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
	return QUrl("https://botfather.io/api/v3/scripts/");
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

	QVector<BotRepo> bot_repo_list;

	for (QJsonValue raw_repo : json.object().value("scripts").toArray())
	{
		if (!raw_repo.isObject())
		{
			qDebug() << "Raw repo is not an object";
			continue;
		}

		QJsonObject raw_repo_object = raw_repo.toObject();
		BotRepo bot_repo = jsonObjectToRepoData(raw_repo_object);

		if (!bot_repo.isValid())
		{
			qDebug() << "Bot repo is invalid:" << bot_repo.name() << bot_repo.gitUrl();
			continue;
		}

		bot_repo_list.append(bot_repo);
	}

	emit scriptsReceived(bot_repo_list);
	emit finished();
}

BotRepo ScriptsApiClient::jsonObjectToRepoData(const QJsonObject &object)
{
	return BotRepo(
		QString::number(object.value("id").toInt()),
		object.value("name").toString(),
		object.value("developer").toString(),
		object.value("description").toString(),
		object.value("platform").toString(),
		object.value("git_clone_url").toString()
	);
}
