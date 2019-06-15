#include "gitprogressdialog.hpp"
#include "ui_gitprogressdialog.h"
#include <QThread>
#include <QPushButton>
#include <QDebug>
#include "../../git/gitcloneoperation.hpp"
#include "../../git/gitrecloneoperation.hpp"

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

void GitProgressDialog::reclone(const QString &repo_url, const QString &dir_path)
{
	setWindowTitle("Script repository update");
	setLabelText("Updating script repository");
	show();

    QThread *reclone_thread = new QThread; // Don't give it a parent, otherwise the app will crash when the parent gets destroyed before the thread finished.
	reclone_thread->setObjectName("GitRecloneOperation Thread");

	GitRecloneOperation *reclone_op = new GitRecloneOperation(repo_url, dir_path);
    reclone_op->moveToThread(reclone_thread);

    connect(reclone_thread, &QThread::started, reclone_op, &GitRecloneOperation::process);
    connect(reclone_op, &GitRecloneOperation::finished, reclone_thread, &QThread::quit);
    connect(reclone_op, &GitRecloneOperation::finished, reclone_op, &GitRecloneOperation::deleteLater);
    connect(reclone_thread, &QThread::finished, reclone_thread, &QThread::deleteLater);

    connect(m_ui->buttonBox, &QDialogButtonBox::rejected, reclone_op, &GitRecloneOperation::cancel);
	connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &GitProgressDialog::reject);
    connect(reclone_op, &GitRecloneOperation::success, this, &GitProgressDialog::cloneSuccess);
    connect(reclone_op, &GitRecloneOperation::failure, this, &GitProgressDialog::cloneFailure);

    connect(reclone_op, &GitRecloneOperation::totalObjectsChanged, this, &GitProgressDialog::setMaximum);
    connect(reclone_op, &GitRecloneOperation::receivedObjectsChanged, this, &GitProgressDialog::setValue);
    connect(reclone_op, &GitRecloneOperation::checkoutTotalChanged, this, &GitProgressDialog::setMaximum);
    connect(reclone_op, &GitRecloneOperation::checkoutCurrentChanged, this, &GitProgressDialog::setValue);
    connect(reclone_op, &GitRecloneOperation::transferProgressChanged, this, &GitProgressDialog::transferProgressChanged);
    connect(reclone_op, &GitRecloneOperation::checkoutProgressChanged, this, &GitProgressDialog::checkoutProgressChanged);

    connect(reclone_op, &GitRecloneOperation::replacingRepo, [this](){
		this->setLabelText("Almost done updating...");
	});

    reclone_thread->start();
}

void GitProgressDialog::clone(const QString &repo_url, const QString &dir_path)
{
	setWindowTitle("Cloning script repository...");
	setLabelText("Cloning script repository...");
	show();

    QThread *clone_thread = new QThread; // Don't give it a parent, otherwise the app will crash when the parent gets destroyed before the thread finished.
	clone_thread->setObjectName("GitCloneOperation Thread");

	GitCloneOperation *clone_op = new GitCloneOperation(repo_url, dir_path);
    clone_op->moveToThread(clone_thread);

    connect(clone_thread, &QThread::started, clone_op, &GitCloneOperation::process);
    connect(clone_op, &GitCloneOperation::finished, clone_thread, &QThread::quit);
    connect(clone_op, &GitCloneOperation::finished, clone_op, &GitCloneOperation::deleteLater);
    connect(clone_thread, &QThread::finished, clone_thread, &QThread::deleteLater);

    connect(m_ui->buttonBox, &QDialogButtonBox::rejected, clone_op, &GitCloneOperation::cancel);
	connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &GitProgressDialog::reject);
    connect(clone_op, &GitCloneOperation::success, this, &GitProgressDialog::cloneSuccess);
    connect(clone_op, &GitCloneOperation::failure, this, &GitProgressDialog::cloneFailure);

    connect(clone_op, &GitCloneOperation::totalObjectsChanged, this, &GitProgressDialog::setMaximum);
    connect(clone_op, &GitCloneOperation::receivedObjectsChanged, this, &GitProgressDialog::setValue);
    connect(clone_op, &GitCloneOperation::checkoutTotalChanged, this, &GitProgressDialog::setMaximum);
    connect(clone_op, &GitCloneOperation::checkoutCurrentChanged, this, &GitProgressDialog::setValue);
    connect(clone_op, &GitCloneOperation::transferProgressChanged, this, &GitProgressDialog::transferProgressChanged);
    connect(clone_op, &GitCloneOperation::checkoutProgressChanged, this, &GitProgressDialog::checkoutProgressChanged);

    clone_thread->start();
}

void GitProgressDialog::cloneSuccess()
{
	setWindowTitle("Script download finished!");
	setLabelText("Script download finished!");

	m_ui->buttonBox->clear();
	QPushButton *btn = m_ui->buttonBox->addButton(QDialogButtonBox::Ok);

	Q_ASSERT(btn);
	connect(btn, &QPushButton::clicked, this, &GitProgressDialog::accept);

	emit cloned();
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
