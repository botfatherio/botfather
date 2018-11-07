#ifndef GITPROGRESS_H
#define GITPROGRESS_H

#include <QObject>
#include <git2.h>

class GitProgress : public QObject
{
	Q_OBJECT
public:
	explicit GitProgress(QObject *parent = nullptr);
	bool isCanceled() const;

	static int transferProgressCallback(const git_transfer_progress *stats, void *git_progress_p);
	static void checkoutProgressCallback(const char *path, size_t cur, size_t tot, void *git_progress_p);

	static const int CANCELED_CODE = 42;

signals:
	void totalObjectsChanged(uint total);
	void receivedObjectsChanged(uint received);
	void transferProgressChanged(uint received, uint total, ulong bytes);
	void checkoutCurrentChanged(ulong current);
	void checkoutTotalChanged(ulong total);
	void checkoutProgressChanged(ulong current, ulong total, const QString &path);

public slots:
	void cancel();

private:
	bool m_canceled = false;
};

#endif // GITPROGRESS_H
