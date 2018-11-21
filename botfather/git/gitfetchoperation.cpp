#include "gitfetchoperation.h"
#include <QDebug>

GitFetchOperation::GitFetchOperation(const QString &repo_path)
	: AbstractGitOperation()
	, m_repo_path(repo_path)
{

}

void GitFetchOperation::process()
{
	git_repository *repo = nullptr;
	git_remote *remote = nullptr;
	int return_code = 0;

	return_code = git_repository_open_ext(&repo, m_repo_path.toUtf8(), GIT_REPOSITORY_OPEN_NO_SEARCH, nullptr);
	if (return_code)
	{
		qDebug() << "Opening the repo failed. ReturnCode:" << return_code << "; RepoPath:" << m_repo_path;
		emit failure(return_code);
		return;
	}

	return_code = git_remote_lookup(&remote, repo, "origin");
	if (return_code != 0)
	{
		qDebug() << "Remote lookup failed. ReturnCode:" << return_code << "; RepoPath:" << m_repo_path;
		emit failure(return_code);
		return;
	}

	git_fetch_options fetch_options = GIT_FETCH_OPTIONS_INIT;
	fetch_options.callbacks.transfer_progress = &AbstractGitOperation::transferProgressCallback;// TODO: find out whether to use & or not

	return_code = git_remote_fetch(remote, nullptr, &fetch_options, "fetch");
	if (return_code != 0)
	{
		qDebug() << "Fetching failed. ReturnCode:" << return_code << "; RepoPath:" << m_repo_path;
		emit failure(return_code);
		return;
	}

	/*
	const git_transfer_progress *final_stats = git_remote_stats(remote);
	final_stats->received_bytes;
	final_stats->indexed_deltas;
	final_stats->total_deltas;
	final_stats->local_objects;
	final_stats->received_objects;
	final_stats->indexed_objects;
	final_stats->total_objects;
	*/

	emit success();
}
