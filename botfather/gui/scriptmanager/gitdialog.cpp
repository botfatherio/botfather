#include "gitdialog.h"
#include "ui_gitdialog.h"
#include <QThread>
#include <QPushButton>
#include <QDebug>
#include "gitworker.h"

GitDialog::GitDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::GitDialog)
{
	ui->setupUi(this);
}

GitDialog::~GitDialog()
{
	delete ui;
}

void GitDialog::setLabelText(const QString &text)
{
	ui->label->setText(text);
}

void GitDialog::setMaximum(int maximum)
{
	ui->progressBar->setMaximum(maximum);
}

void GitDialog::setValue(int value)
{
	ui->progressBar->setValue(value);
}

void GitDialog::clone(const QString &repo_url, const QString &dir_path)
{
	setWindowTitle("Cloning script repository...");
	setLabelText("Cloning script repository...");
	show();

	QThread *thread = new QThread;
	GitWorker *worker = new GitWorker(repo_url, dir_path);
	worker->moveToThread(thread);

	connect(thread, &QThread::started, worker, &GitWorker::process);
	connect(worker, &GitWorker::finished, thread, &QThread::quit);
	connect(worker, &GitWorker::finished, worker, &GitWorker::deleteLater);
	connect(thread, &QThread::finished, thread, &QThread::deleteLater);

	connect(ui->buttonBox, &QDialogButtonBox::rejected, worker, &GitWorker::cancel);
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &GitDialog::reject);
	connect(worker, &GitWorker::success, this, &GitDialog::cloneSuccess);
	connect(worker, &GitWorker::failure, this, &GitDialog::cloneFailure);

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

void GitDialog::cloneSuccess()
{
	setLabelText("Script downloaded");
	ui->buttonBox->clear();

	QPushButton *btn = ui->buttonBox->addButton(QDialogButtonBox::Ok);
	if (!btn) return;

	// The buttons role doesn't matter, we decide what signal is triggered.
	connect(btn, &QPushButton::clicked, this, &GitDialog::accept);
}

void GitDialog::cloneFailure()
{
	setLabelText("Script download failed");
	ui->buttonBox->clear();

	QPushButton *btn = ui->buttonBox->addButton(QDialogButtonBox::Close);
	if (!btn) return;

	// The buttons role doesn't matter, we decide what signal is triggered.
	connect(btn, &QPushButton::clicked, this, &GitDialog::reject);
}