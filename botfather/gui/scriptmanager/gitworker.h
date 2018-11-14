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

	uint totalObjects() const;
	void setTotalObjects(uint objects);

	uint receivedObjects() const;
	void setReceivedObjects(uint objects);

	ulong receivedBytes() const;
	void setReceivedBytes(ulong bytes);

	ulong completedSteps() const;
	void setCompletedSteps(ulong steps);

	ulong totalSteps() const;
	void setTotalSteps(ulong steps);

	QString checkoutPath() const;
	void setCheckoutPath(const QString &path);

public slots:
	void cancel();
	void process();

signals:
	void success();
	void failure();
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

	// Transfer // TODO: maybe move this into a dedicated GitTransferProgress class
	uint m_total_objects = 0;
	uint m_received_objects = 0;
	ulong m_received_bytes = 0;

	// Checkout // TODO: maybe move this into a dedicated GitCheckoutProgress class
	ulong m_completed_steps = 0;
	ulong m_total_steps = 0;
	QString m_checkout_path;
};

#endif // GITWORKER_H
