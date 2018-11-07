#ifndef GITWORKER_H
#define GITWORKER_H

#include <QObject>
#include <git2.h>

class GitWorker : public QObject
{
	Q_OBJECT
public:
	explicit GitWorker(const QString &repo_url, const QString &dir_path);
	~GitWorker();

	static const int CANCELED_CODE = 42;

	static int transferProgressCallback(const git_transfer_progress *stats, void *git_worker_p);
	static void checkoutProgressCallback(const char *path, size_t cur, size_t tot, void *git_worker_p);

	bool isCanceled() const;

public slots:
	void cancel();
	void process();

signals:
	void error(int error_code);
	void finished();

	void totalObjectsChanged(uint total);
	void receivedObjectsChanged(uint received);
	void transferProgressChanged(uint received, uint total, ulong bytes);
	void checkoutCurrentChanged(ulong current);
	void checkoutTotalChanged(ulong total);
	void checkoutProgressChanged(ulong current, ulong total, const QString &path);

private:
	bool m_is_canceled = false;
	QString m_repo_url;
	QString m_dir_path;
};

#endif // GITWORKER_H
