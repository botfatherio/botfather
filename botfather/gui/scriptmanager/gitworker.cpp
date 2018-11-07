#include "gitworker.h"
#include <QDebug>

GitWorker::GitWorker(const QString &repo_url, const QString &dir_path)
	: QObject()
	, m_repo_url(repo_url)
	, m_dir_path(dir_path)
{
	git_libgit2_init();
}

GitWorker::~GitWorker()
{
	git_libgit2_shutdown();
}

int GitWorker::transferProgressCallback(const git_transfer_progress *stats, void *git_worker_p)
{
	GitWorker *worker = qobject_cast<GitWorker*>(static_cast<QObject*>(git_worker_p));

	if (worker->isCanceled())
	{
		return CANCELED_CODE;
	}

	emit worker->totalObjectsChanged(stats->total_objects);
	emit worker->receivedObjectsChanged(stats->received_objects);
	emit worker->transferProgressChanged(stats->received_objects, stats->total_objects, stats->received_bytes);

	return 0;
}

void GitWorker::checkoutProgressCallback(const char *path, size_t current, size_t total, void *git_worker_p)
{
	GitWorker *worker = qobject_cast<GitWorker*>(static_cast<QObject*>(git_worker_p));
	emit worker->checkoutCurrentChanged(current);
	emit worker->checkoutTotalChanged(total);
	emit worker->checkoutProgressChanged(current, total, path);
}

bool GitWorker::isCanceled() const
{
	return m_is_canceled;
}

void GitWorker::cancel()
{
	m_is_canceled = true;
}

void GitWorker::process()
{

	git_repository *repo = nullptr;
	git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;

	clone_opts.fetch_opts.callbacks.payload = this;
	clone_opts.fetch_opts.callbacks.transfer_progress = GitWorker::transferProgressCallback;
	clone_opts.checkout_opts.checkout_strategy = GIT_CHECKOUT_FORCE; // git checkout --force; all modifications are overwritten, and all missing files are created.
	clone_opts.checkout_opts.progress_cb = GitWorker::checkoutProgressCallback;
	clone_opts.checkout_opts.progress_payload = this;

	int code = git_clone(&repo, m_repo_url.toUtf8(), m_dir_path.toUtf8(), &clone_opts);
	qDebug() << "Git clone code:" << code << (code == 0 ? "" : giterr_last()->message);

	if (code != 0)
	{
		emit error(code);
	}

	git_repository_free(repo);
	emit finished();
}
