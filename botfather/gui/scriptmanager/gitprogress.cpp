#include "gitprogress.h"
#include <QDebug>

GitProgress::GitProgress(QObject *parent) : QObject(parent)
{

}

bool GitProgress::isCanceled() const
{
	return m_canceled;
}

int GitProgress::transferProgressCallback(const git_transfer_progress *stats, void *git_progress_p)
{
	GitProgress *progress = qobject_cast<GitProgress*>(static_cast<QObject*>(git_progress_p));

	if (progress->isCanceled())
	{
		return CANCELED_CODE;
	}

	emit progress->totalObjectsChanged(stats->total_objects);
	emit progress->receivedObjectsChanged(stats->received_objects);
	emit progress->transferProgressChanged(stats->received_objects, stats->total_objects, stats->received_bytes);

	return 0;
}

void GitProgress::checkoutProgressCallback(const char *path, size_t cur, size_t tot, void *git_progress_p)
{
	GitProgress *progress = qobject_cast<GitProgress*>(static_cast<QObject*>(git_progress_p));
	emit progress->checkoutCurrentChanged(cur);
	emit progress->checkoutTotalChanged(tot);
	emit progress->checkoutProgressChanged(cur, tot, path);
	qDebug() << "emitted";
}

void GitProgress::cancel()
{
	m_canceled = true;
}
