#include "botrepo.h"
#include <QFileInfo>
#include <QUrl>
#include <QDebug>
#include <QThread>
#include <QDir>

BotRepo::BotRepo(QObject *parent)
	: QObject(parent)
{

}

BotRepo::BotRepo(BotRepo::Data data, QObject *parent)
	: BotRepo(parent)
{
	m_data = data;
}

bool BotRepo::isValid() const
{
	return !remoteUrl().isEmpty() && QUrl(remoteUrl()).isValid();
}

QString BotRepo::findScriptPath() const
{
	QDir repo_dir(localPath());

	if (localPath().isEmpty() || repo_dir.isEmpty())
	{
		// Invalid local repo
		return QString();
	}

	QStringList scriptname_filters;
	scriptname_filters << "*.js";

	// Don't check for readability yet, the bot engine informs the user about permission issues
	QStringList entries = repo_dir.entryList(scriptname_filters, QDir::Files|QDir::NoDotAndDotDot);

	if (entries.isEmpty())
	{
		// No script found
		return QString();
	}

	return repo_dir.filePath(entries.first());
}

BotRepo::Data BotRepo::data() const
{
	return m_data;
}

QString BotRepo::name() const
{
	return m_data.name;
}

void BotRepo::setName(const QString &name)
{
	m_data.name = name;
}

QString BotRepo::developer() const
{
	return m_data.developer;
}

void BotRepo::setDeveloper(const QString &developer)
{
	m_data.developer = developer;
}

QString BotRepo::description() const
{
	return m_data.description;
}

void BotRepo::setDescription(const QString &description)
{
	m_data.description = description;
}

QString BotRepo::localPath() const
{
	return m_data.local_path;
}

void BotRepo::setLocalPath(const QString &path)
{
	m_data.local_path = path;
}

QString BotRepo::remoteUrl() const
{
	return m_data.remote_url;
}

void BotRepo::setRemoteUrl(const QString &url)
{
	m_data.remote_url = url;
}
