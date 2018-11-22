#include "gitrecloneoperation.h"
#include "gitcloneoperation.h"
#include <QDir>

GitRecloneOperation::GitRecloneOperation(const QString &remote_url, const QString &local_path)
	: AbstractGitOperation()
	, m_remote_url(remote_url)
	, m_local_path(local_path)
{
	m_tmp_clone_dir = QDir::temp();
}

void GitRecloneOperation::process()
{
	GitCloneOperation *clone_op = new GitCloneOperation(m_remote_url, m_tmp_clone_dir.path());

	connect(clone_op, SIGNAL(failure()), this, SIGNAL(failure()));
	connect(clone_op, SIGNAL(success()), this, SLOT(replaceRepo()));
	connect(clone_op, SIGNAL(finished()), clone_op, SLOT(deleteLater()));
}

void GitRecloneOperation::replaceRepo()
{
	emit replacingRepo();

	QDir repo_dir(m_local_path);
	repo_dir.removeRecursively();

	if (!m_tmp_clone_dir.rename(m_tmp_clone_dir.path(), m_local_path))
	{
		emit failure();
		return;
	}

	emit success();
}
