#include "gitrecloneoperation.h"
#include "gitcloneoperation.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>

GitRecloneOperation::GitRecloneOperation(const QString &remote_url, const QString &local_path)
	: AbstractGitOperation()
	, m_remote_url(remote_url)
	, m_local_path(local_path)
{
	m_tmp_clone_dir.setAutoRemove(false);
}

void GitRecloneOperation::process()
{
	GitCloneOperation *clone_op = new GitCloneOperation(m_remote_url, m_tmp_clone_dir.path());

	connect(clone_op, SIGNAL(failure()), this, SIGNAL(failure()));
	connect(clone_op, SIGNAL(success()), this, SLOT(replaceRepo()));
	connect(clone_op, SIGNAL(finished()), clone_op, SLOT(deleteLater()));

	connect(clone_op, &GitCloneOperation::totalObjectsChanged, this, &GitRecloneOperation::totalObjectsChanged);
	connect(clone_op, &GitCloneOperation::receivedObjectsChanged, this, &GitRecloneOperation::receivedObjectsChanged);
	connect(clone_op, &GitCloneOperation::checkoutTotalChanged, this, &GitRecloneOperation::checkoutTotalChanged);
	connect(clone_op, &GitCloneOperation::checkoutCurrentChanged, this, &GitRecloneOperation::checkoutCurrentChanged);
	connect(clone_op, &GitCloneOperation::transferProgressChanged, this, &GitRecloneOperation::transferProgressChanged);
	connect(clone_op, &GitCloneOperation::checkoutProgressChanged, this, &GitRecloneOperation::checkoutProgressChanged);

	clone_op->process();
}

static void moveDir(const QString &source_path, const QString &dest_path)
{
	QDir source_dir(source_path);
	Q_ASSERT(source_dir.exists());

	QDir dest_dir(dest_path);
	if (!dest_dir.exists())
	{
		qDebug() << "Dest dir doesn't exist, create it" << dest_path;
		dest_dir.mkpath(".");
	}

	foreach (QFileInfo info, source_dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden))
	{
		QString old_path = info.absoluteFilePath();
		QString new_path = QString("%1/%2").arg(dest_path).arg(info.fileName());

		if (info.isDir())
		{
			qDebug() << "Move dir" << old_path << "to" << new_path;
			moveDir(old_path, new_path);
		}
		else
		{
			qDebug() << "Move file" << old_path << "to" << new_path;
			QFile::rename(old_path, new_path);
		}
	}
}

void GitRecloneOperation::replaceRepo()
{
	emit replacingRepo();

	QDir repo_dir(m_local_path);

	// Preserve the bots .settings.ini and .config.ini files
	QStringList files_to_preserve;
	files_to_preserve << ".settings.ini" << ".config.ini";

	for (const QString &file_name : files_to_preserve)
	{
		qDebug() << "Trying to preserve" << file_name;
		QFile::copy(repo_dir.filePath(file_name), m_tmp_clone_dir.filePath(file_name));
	}

	// Replace the bot folders contents with the updated contents including the preserved files
	repo_dir.removeRecursively();
	moveDir(m_tmp_clone_dir.path(), repo_dir.path());

	emit success();
}
