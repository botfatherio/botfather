#include "gitprogressdialog.h"
#include "ui_gitprogressdialog.h"
#include <QThread>
#include <QPushButton>
#include <QDebug>
#include "../../git/gitcloneoperation.h"
#include "../../git/gitrecloneoperation.h"

GitProgressDialog::GitProgressDialog(QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::GitProgressDialog)
{
	m_ui->setupUi(this);
}

GitProgressDialog::~GitProgressDialog()
{
	delete m_ui;
}

void GitProgressDialog::setLabelText(const QString &text)
{
	m_ui->label->setText(text);
}

void GitProgressDialog::setMaximum(int maximum)
{
	m_ui->progressBar->setMaximum(maximum);
}

void GitProgressDialog::setValue(int value)
{
	m_ui->progressBar->setValue(value);
}

void GitProgressDialog::transferProgressChanged(uint received, uint total, uint bytes)
{
	Q_UNUSED(bytes)
	QString label_text = QString("Downloading objects: %1/%2 done, %3 KiB").arg(received).arg(total).arg(bytes / 1024);
	setLabelText(label_text);
}

void GitProgressDialog::checkoutProgressChanged(ulong current, ulong total, const QString &path)
{
	Q_UNUSED(path)
	QString label_text = QString("Checking out files: %1/%2 done.").arg(current).arg(total);
	setLabelText(label_text);
}

void GitProgressDialog::reclone(ScriptRepository *repository)
{
	setWindowTitle("Script repository update");
	setLabelText("Updating script repository");
	show();

	QThread *thread = new QThread; // Don't give it a parent, otherwise the app will crash when the parent gets destroyed before the thread finished.
	GitRecloneOperation *operation = new GitRecloneOperation(repository->remoteUrl(), repository->localPath());
	operation->moveToThread(thread);

	connect(thread, &QThread::started, operation, &GitRecloneOperation::process);
	connect(operation, &GitRecloneOperation::finished, thread, &QThread::quit);
	connect(operation, &GitRecloneOperation::finished, operation, &GitRecloneOperation::deleteLater);
	connect(thread, &QThread::finished, thread, &QThread::deleteLater);

	connect(m_ui->buttonBox, &QDialogButtonBox::rejected, operation, &GitRecloneOperation::cancel);
	connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &GitProgressDialog::reject);
	connect(operation, &GitRecloneOperation::success, this, &GitProgressDialog::cloneSuccess);
	connect(operation, &GitRecloneOperation::failure, this, &GitProgressDialog::cloneFailure);

	connect(operation, &GitRecloneOperation::totalObjectsChanged, this, &GitProgressDialog::setMaximum);
	connect(operation, &GitRecloneOperation::receivedObjectsChanged, this, &GitProgressDialog::setValue);
	connect(operation, &GitRecloneOperation::checkoutTotalChanged, this, &GitProgressDialog::setMaximum);
	connect(operation, &GitRecloneOperation::checkoutCurrentChanged, this, &GitProgressDialog::setValue);
	connect(operation, &GitRecloneOperation::transferProgressChanged, this, &GitProgressDialog::transferProgressChanged);
	connect(operation, &GitRecloneOperation::checkoutProgressChanged, this, &GitProgressDialog::checkoutProgressChanged);

	connect(operation, &GitRecloneOperation::replacingRepo, [this](){
		this->setLabelText("Almost done updating...");
	});

	thread->start();
}

void GitProgressDialog::clone(ScriptRepository *repository)
{
	m_dest_repo = repository;

	setWindowTitle("Cloning script repository...");
	setLabelText("Cloning script repository...");
	show();

	QThread *thread = new QThread; // Don't give it a parent, otherwise the app will crash when the parent gets destroyed before the thread finished.
	GitCloneOperation *operation = new GitCloneOperation(repository->remoteUrl(), repository->localPath());
	operation->moveToThread(thread);

	connect(thread, &QThread::started, operation, &GitCloneOperation::process);
	connect(operation, &GitCloneOperation::finished, thread, &QThread::quit);
	connect(operation, &GitCloneOperation::finished, operation, &GitCloneOperation::deleteLater);
	connect(thread, &QThread::finished, thread, &QThread::deleteLater);

	connect(m_ui->buttonBox, &QDialogButtonBox::rejected, operation, &GitCloneOperation::cancel);
	connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &GitProgressDialog::reject);
	connect(operation, &GitCloneOperation::success, this, &GitProgressDialog::cloneSuccess);
	connect(operation, &GitCloneOperation::failure, this, &GitProgressDialog::cloneFailure);

	connect(operation, &GitCloneOperation::totalObjectsChanged, this, &GitProgressDialog::setMaximum);
	connect(operation, &GitCloneOperation::receivedObjectsChanged, this, &GitProgressDialog::setValue);
	connect(operation, &GitCloneOperation::checkoutTotalChanged, this, &GitProgressDialog::setMaximum);
	connect(operation, &GitCloneOperation::checkoutCurrentChanged, this, &GitProgressDialog::setValue);
	connect(operation, &GitCloneOperation::transferProgressChanged, this, &GitProgressDialog::transferProgressChanged);
	connect(operation, &GitCloneOperation::checkoutProgressChanged, this, &GitProgressDialog::checkoutProgressChanged);

	thread->start();
}

void GitProgressDialog::cloneSuccess()
{
	setWindowTitle("Script download finished!");
	setLabelText("Script download finished!");

	m_ui->buttonBox->clear();
	QPushButton *btn = m_ui->buttonBox->addButton(QDialogButtonBox::Ok);

	Q_ASSERT(btn);
	connect(btn, &QPushButton::clicked, this, &GitProgressDialog::accept);

	emit cloned(m_dest_repo);
}

void GitProgressDialog::cloneFailure()
{
	setWindowTitle("Script download failed!");
	setLabelText("Script download failed!");

	m_ui->buttonBox->clear();
	QPushButton *btn = m_ui->buttonBox->addButton(QDialogButtonBox::Close);

	Q_ASSERT(btn);
	connect(btn, &QPushButton::clicked, this, &GitProgressDialog::reject);
}
