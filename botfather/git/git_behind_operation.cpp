#include "git_behind_operation.hpp"
#include <QDebug>

GitBehindOperation::GitBehindOperation(const QString &repo_path)
	: AbstractGitOperation()
	, m_repo_path(repo_path)
{

}

void GitBehindOperation::process()
{
	git_repository *repo = nullptr;

	int return_code = git_repository_open(&repo, m_repo_path.toUtf8());
	if (return_code)
	{
		qDebug() << "Failed to open repo. ReturnCode:" << return_code << "; RepoPath:" << m_repo_path;
		emit failure(return_code);
		return;
	}

	git_revwalk *refwalk;
	git_revwalk_new(&refwalk, repo);
	git_revwalk_push_ref(refwalk, "refs/remotes/origin/master"); // <- remote
	git_revwalk_hide_ref(refwalk, "refs/heads/master"); // <- local

	git_oid id;
	int diffs_count = 0;

	while (!git_revwalk_next(&id, refwalk))
	{
		++diffs_count;
	}

	qDebug() << "Differences to remote:" << diffs_count;
	git_repository_free(repo);

	emit differencesToRemote(diffs_count);
	emit success();
}
