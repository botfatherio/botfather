#include "gitworker.h"
#include <QCoreApplication>
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
	QCoreApplication::processEvents(); // Without it the worker will miss cancel requests

	if (worker->isCanceled())
	{
		return CANCELED_CODE;
	}

	if (stats->total_objects != worker->totalObjects())
	{
		worker->setTotalObjects(stats->total_objects);
		emit worker->totalObjectsChanged(stats->total_objects);
	}

	if (stats->received_objects != worker->receivedObjects())
	{
		worker->setReceivedObjects(stats->received_objects);
		emit worker->receivedObjectsChanged(stats->received_objects);
	}

	if (stats->received_bytes != worker->receivedBytes())
	{
		worker->setReceivedBytes(stats->received_bytes);
		emit worker->transferProgressChanged(stats->received_objects, stats->total_objects, stats->received_bytes);
	}

	return 0;
}

void GitWorker::checkoutProgressCallback(const char *path, size_t completed_steps, size_t total_steps, void *git_worker_p)
{
	GitWorker *worker = qobject_cast<GitWorker*>(static_cast<QObject*>(git_worker_p));
	QCoreApplication::processEvents(); // Without it the worker will miss cancel requests

	if (completed_steps != worker->completedSteps())
	{
		worker->setCompletedSteps(completed_steps);
		emit worker->checkoutCurrentChanged(completed_steps);
	}

	if (total_steps != worker->totalSteps())
	{
		worker->setTotalSteps(total_steps);
		emit worker->checkoutTotalChanged(total_steps);
	}

	if (path != worker->checkoutPath())
	{
		worker->setCheckoutPath(path);
		emit worker->checkoutProgressChanged(completed_steps, total_steps, path);
	}
}

bool GitWorker::isCanceled() const
{
	return m_is_canceled;
}

uint GitWorker::totalObjects() const
{
	return m_total_objects;
}

void GitWorker::setTotalObjects(uint objects)
{
	m_total_objects = objects;
}

uint GitWorker::receivedObjects() const
{
	return m_received_objects;
}

void GitWorker::setReceivedObjects(uint objects)
{
	m_received_objects = objects;
}

ulong GitWorker::receivedBytes() const
{
	return m_received_bytes;
}

void GitWorker::setReceivedBytes(ulong bytes)
{
	m_received_bytes = bytes;
}

ulong GitWorker::completedSteps() const
{
	return m_completed_steps;
}

void GitWorker::setCompletedSteps(ulong steps)
{
	m_completed_steps = steps;
}

ulong GitWorker::totalSteps() const
{
	return m_total_steps;
}

void GitWorker::setTotalSteps(ulong steps)
{
	m_total_steps = steps;
}

QString GitWorker::checkoutPath() const
{
	return m_checkout_path;
}

void GitWorker::setCheckoutPath(const QString &path)
{
	m_checkout_path = path;
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
