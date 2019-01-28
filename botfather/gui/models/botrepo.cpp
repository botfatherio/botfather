#include "botrepo.h"
#include <QFileInfo>
#include <QUrl>
#include <QDebug>
#include <QThread>
#include <QDir>
#include <git2.h>
#include "../../git/gitfetchoperation.h"
#include "../../git/gitbehindoperation.h"

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
	if (localPath().isEmpty())
	{
		return !remoteUrl().isEmpty() && QUrl(remoteUrl()).isValid();
	}

	// Pass nullptr for the output parameter to check for but not open the repo
    return git_repository_open_ext(nullptr, localPath().toUtf8(), GIT_REPOSITORY_OPEN_NO_SEARCH, nullptr) == 0;
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

BotRepo::Status BotRepo::status() const
{
	return m_status;
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

void BotRepo::checkStatus()
{
	// To check whether the script is outdated we first have to fetch from the remote.
	// After doing so we can calculate the differences to the remote.

    QThread *fetch_thread = new QThread; // Don't give it a parent, otherwise the app will crash when the parent gets destroyed before the thread finished.
    fetch_thread->setObjectName(QString("GitFetchOperation Thread for %0").arg(localPath()));

    GitFetchOperation *fetch_op = new GitFetchOperation(localPath());
	fetch_op->moveToThread(fetch_thread);

	connect(fetch_thread, &QThread::started, fetch_op, &GitFetchOperation::process);
	connect(fetch_op, &GitFetchOperation::finished, fetch_thread, &QThread::quit);
	connect(fetch_op, &GitFetchOperation::finished, fetch_op, &GitFetchOperation::deleteLater);
	connect(fetch_thread, &QThread::finished, fetch_thread, &QThread::deleteLater);

    QThread *behind_thread = new QThread; // Don't give it a parent, otherwise the app will crash when the parent gets destroyed before the thread finished.
    behind_thread->setObjectName(QString("GitBehindOperation Thread for %0").arg(localPath()));

    GitBehindOperation *behind_op = new GitBehindOperation(localPath());
    behind_op->moveToThread(behind_thread);

	connect(behind_thread, &QThread::started, behind_op, &GitBehindOperation::process);
	connect(behind_op, &GitBehindOperation::finished, behind_thread, &QThread::quit);
	connect(behind_op, &GitBehindOperation::finished, behind_op, &GitBehindOperation::deleteLater);
	connect(behind_thread, &QThread::finished, behind_thread, &QThread::deleteLater);

	connect(behind_op, &GitBehindOperation::differencesToRemote, this, &BotRepo::noteDifferencesToRemote);
	connect(fetch_op, SIGNAL(finished()), behind_thread, SLOT(start()));

	fetch_thread->start();
}

void BotRepo::noteDifferencesToRemote(int differences)
{
	m_status = differences > 0 ? Status::Outdated : Status::UpToDate;
}
