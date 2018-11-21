#include "gitcloneoperation.h"
#include <QCoreApplication>
#include <QDebug>

GitCloneOperation::GitCloneOperation(const QString &repo_url, const QString &dir_path)
	: AbstractGitOperation()
	, m_repo_url(repo_url)
	, m_dir_path(dir_path)
{

}

void GitCloneOperation::process()
{
	git_repository *repo = nullptr;
	git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;

	clone_opts.fetch_opts.callbacks.payload = this;
	clone_opts.fetch_opts.callbacks.transfer_progress = GitCloneOperation::transferProgressCallback;
	clone_opts.checkout_opts.checkout_strategy = GIT_CHECKOUT_FORCE; // git checkout --force; all modifications are overwritten, and all missing files are created.
	clone_opts.checkout_opts.progress_cb = &AbstractGitOperation::checkoutProgressCallback;
	clone_opts.checkout_opts.progress_payload = this;

	int return_code = git_clone(&repo, m_repo_url.toUtf8(), m_dir_path.toUtf8(), &clone_opts);
	git_repository_free(repo);

	if (return_code)
	{
		emit failure(return_code);
		qDebug() << "Cloning repo failed. ReturnCode:" << return_code;
		return;
	}

	emit success();
}
