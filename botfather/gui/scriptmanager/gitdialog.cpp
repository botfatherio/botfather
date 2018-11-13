#include "gitdialog.h"
#include <QThread>
#include <QDebug>
#include "gitworker.h"

GitDialog::GitDialog(QWidget *parent) : QProgressDialog(parent)
{
	setWindowModality(Qt::WindowModal);
	setMinimumWidth(320);
	setCancelButtonText("Cancel");

	// We use the same QProgressDialog to show different processes in sequence.
	// The clone process for instance first transferns all files and then does the checkout.
	// We don't want to dialog the close or reset between differen processes.
	setAutoClose(false);
	setAutoReset(false);
}

void GitDialog::clone(const QString &repo_url, const QString &dir_path)
{
	setWindowTitle("Cloning script repository...");
	setLabelText("Cloning script repository...");

	QThread *thread = new QThread;
	GitWorker *worker = new GitWorker(repo_url, dir_path);
	worker->moveToThread(thread);

	connect(thread, &QThread::started, worker, &GitWorker::process);
	connect(worker, &GitWorker::finished, thread, &QThread::quit);
	connect(worker, &GitWorker::finished, worker, &GitWorker::deleteLater);
	connect(thread, &QThread::finished, thread, &QThread::deleteLater);

	connect(this, &GitDialog::canceled, worker, &GitWorker::cancel);
	connect(worker, &GitWorker::finished, this, &QProgressDialog::close);

	connect(worker, &GitWorker::totalObjectsChanged, this, &GitDialog::setMaximum);
	connect(worker, &GitWorker::receivedObjectsChanged, this, &GitDialog::setValue);
	connect(worker, &GitWorker::checkoutTotalChanged, this, &GitDialog::setMaximum);
	connect(worker, &GitWorker::checkoutCurrentChanged, this, &GitDialog::setValue);
	connect(worker, &GitWorker::transferProgressChanged, this, &GitDialog::transferProgressChanged);
	connect(worker, &GitWorker::checkoutProgressChanged, this, &GitDialog::checkoutProgressChanged);

	thread->start();
}

void GitDialog::transferProgressChanged(uint received, uint total, uint bytes)
{
	Q_UNUSED(bytes)
	QString label_text = QString("Downloading objects: %1/%2 done.").arg(received).arg(total); // TODO: show bytes downloaded
	setLabelText(label_text);
}

void GitDialog::checkoutProgressChanged(ulong current, ulong total, const QString &path)
{
	Q_UNUSED(path)
	QString label_text = QString("Checking out files: %1/%2 done.").arg(current).arg(total);
	setLabelText(label_text);
}
