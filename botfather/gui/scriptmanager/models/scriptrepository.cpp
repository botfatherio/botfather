#include "scriptrepository.h"
#include <QFileInfo>
#include <QUrl>
#include <QDebug>
#include <QThread>
#include <QDir>
#include <git2.h>
#include "../../../git/gitfetchoperation.h"
#include "../../../git/gitbehindoperation.h"

ScriptRepository::ScriptRepository(QObject *parent)
	: QObject(parent)
{

}

ScriptRepository::ScriptRepository(ScriptRepository::Data data, QObject *parent)
	: QObject(parent)
	, m_data(data)
{

}

bool ScriptRepository::isValid() const
{
	if (localPath().isEmpty())
	{
		return !remoteUrl().isEmpty() && QUrl(remoteUrl()).isValid();
	}

	git_libgit2_init();

	// Pass nullptr for the output parameter to check for but not open the repo
	bool valid_repo = git_repository_open_ext(nullptr, localPath().toUtf8(), GIT_REPOSITORY_OPEN_NO_SEARCH, nullptr) == 0;

	git_libgit2_shutdown();
	return valid_repo;
}

QString ScriptRepository::findScriptPath() const
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

ScriptRepository::Status ScriptRepository::status() const
{
	return m_status;
}

ScriptRepository::Data ScriptRepository::data() const
{
	return m_data;
}

QString ScriptRepository::name() const
{
	return m_data.name;
}

void ScriptRepository::setName(const QString &name)
{
	m_data.name = name;
}

QString ScriptRepository::developer() const
{
	return m_data.developer;
}

void ScriptRepository::setDeveloper(const QString &developer)
{
	m_data.developer = developer;
}

QString ScriptRepository::description() const
{
	return m_data.description;
}

void ScriptRepository::setDescription(const QString &description)
{
	m_data.description = description;
}

QString ScriptRepository::localPath() const
{
	return m_data.local_path;
}

void ScriptRepository::setLocalPath(const QString &path)
{
	m_data.local_path = path;
}

QString ScriptRepository::remoteUrl() const
{
	return m_data.remote_url;
}

void ScriptRepository::setRemoteUrl(const QString &url)
{
	m_data.remote_url = url;
}

void ScriptRepository::checkStatus()
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

	connect(behind_op, &GitBehindOperation::differencesToRemote, this, &ScriptRepository::noteDifferencesToRemote);
	connect(fetch_op, SIGNAL(finished()), behind_thread, SLOT(start()));

	fetch_thread->start();
}

void ScriptRepository::noteDifferencesToRemote(int differences)
{
	m_status = differences > 0 ? Status::Outdated : Status::UpToDate;
}
