#include "gitdialog.h"
#include "ui_gitdialog.h"
#include <QThread>
#include <QPushButton>
#include <QDebug>
#include "../../git/gitcloneoperation.h"

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

void GitDialog::transferProgressChanged(uint received, uint total, uint bytes)
{
	Q_UNUSED(bytes)
	QString label_text = QString("Downloading objects: %1/%2 done, %3 KiB").arg(received).arg(total).arg(bytes / 1024);
	setLabelText(label_text);
}

void GitDialog::checkoutProgressChanged(ulong current, ulong total, const QString &path)
{
	Q_UNUSED(path)
	QString label_text = QString("Checking out files: %1/%2 done.").arg(current).arg(total);
	setLabelText(label_text);
}

void GitDialog::clone(ScriptRepository *repository, const QString &local_path)
{
	dest_repo = new ScriptRepository(repository->data());
	dest_repo->setRepository(local_path);

	setWindowTitle("Cloning script repository...");
	setLabelText("Cloning script repository...");
	show();

	QThread *thread = new QThread;
	GitCloneOperation *operation = new GitCloneOperation(repository->repository(), local_path);
	operation->moveToThread(thread);

	connect(thread, &QThread::started, operation, &GitCloneOperation::process);
	connect(operation, &GitCloneOperation::finished, thread, &QThread::quit);
	connect(operation, &GitCloneOperation::finished, operation, &GitCloneOperation::deleteLater);
	connect(thread, &QThread::finished, thread, &QThread::deleteLater);

	connect(ui->buttonBox, &QDialogButtonBox::rejected, operation, &GitCloneOperation::cancel);
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &GitDialog::reject);
	connect(operation, &GitCloneOperation::success, this, &GitDialog::cloneSuccess);
	connect(operation, &GitCloneOperation::failure, this, &GitDialog::cloneFailure);

	connect(operation, &GitCloneOperation::totalObjectsChanged, this, &GitDialog::setMaximum);
	connect(operation, &GitCloneOperation::receivedObjectsChanged, this, &GitDialog::setValue);
	connect(operation, &GitCloneOperation::checkoutTotalChanged, this, &GitDialog::setMaximum);
	connect(operation, &GitCloneOperation::checkoutCurrentChanged, this, &GitDialog::setValue);
	connect(operation, &GitCloneOperation::transferProgressChanged, this, &GitDialog::transferProgressChanged);
	connect(operation, &GitCloneOperation::checkoutProgressChanged, this, &GitDialog::checkoutProgressChanged);

	thread->start();
}

void GitDialog::cloneSuccess()
{
	setWindowTitle("Script download finished!");
	setLabelText("Script download finished!");

	ui->buttonBox->clear();
	QPushButton *btn = ui->buttonBox->addButton(QDialogButtonBox::Ok);

	Q_ASSERT(btn);
	connect(btn, &QPushButton::clicked, this, &GitDialog::accept);

	emit cloned(dest_repo);
}

void GitDialog::cloneFailure()
{
	setWindowTitle("Script download failed!");
	setLabelText("Script download failed!");

	ui->buttonBox->clear();
	QPushButton *btn = ui->buttonBox->addButton(QDialogButtonBox::Close);

	Q_ASSERT(btn);
	connect(btn, &QPushButton::clicked, this, &GitDialog::reject);
}
