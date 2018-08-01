#include "updater_dialog.h"
#include "ui_updater_dialog.h"
#include <QPushButton>
#include <QMessageBox>
#include <QThread>
#include <QDebug>

UpdaterDialog::UpdaterDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::UpdaterDialog)
{
	ui->setupUi(this);
	ui->button_box->button(QDialogButtonBox::Ok)->setHidden(true);
	
	QThread *checker_thread = new QThread(this);
	update_checker = new UpdateChecker;
	update_checker->moveToThread(checker_thread);
	
	connect(checker_thread, SIGNAL(started()), update_checker, SLOT(checkForUpdates()));
	connect(this, SIGNAL(finished(int)), checker_thread, SLOT(quit()));
	connect(this, SIGNAL(finished(int)), update_checker, SLOT(deleteLater()));
	connect(checker_thread, SIGNAL(finished()), checker_thread, SLOT(deleteLater()));
	
	connect(ui->button_box, SIGNAL(rejected()), this, SLOT(reject()));
	connect(this, SIGNAL(rejected()), update_checker, SLOT(cancelUpdateCheck()));
	connect(this, SIGNAL(rejected()), this, SLOT(disconnectUpdateChecker()));
	
	connect(update_checker, SIGNAL(updatesAvailable()), this, SLOT(onUpdatesAvailable()));
	connect(update_checker, SIGNAL(noUpdatesAvailable()), this, SLOT(accept()));
	connect(update_checker, SIGNAL(updateCheckFailed(UpdateChecker::ErrorType)), this, SLOT(onUpdateCheckFailed(UpdateChecker::ErrorType)));
	
	checker_thread->start();
}

UpdaterDialog::~UpdaterDialog()
{
	delete ui;
}

void UpdaterDialog::onUpdatesAvailable()
{
	QString message(
		"There are updates available!<br>"
		"Manually start the <strong>maintainancetool</strong> to download and install them. "
		"Do you want to close BotFather to do so?"
	);
	
	QMessageBox::StandardButton reply = QMessageBox::question(
		this,
		"Updates Available",
		message,
		QMessageBox::Yes|QMessageBox::No,
		QMessageBox::Yes
	);
	
	if (reply == QMessageBox::Yes) {
		qApp->quit();
	}
	else {
		// The updater dialog is accepted here, not the question. Hint: Look at the connections we've set up
		// and you'll understand why we don't want to reject here.
		accept();
	}
}

void UpdaterDialog::onUpdateCheckFailed(UpdateChecker::ErrorType error_type)
{
	QMessageBox *message_box = new QMessageBox;
	message_box->setIcon(QMessageBox::Warning);
	message_box->setWindowTitle("Checking for updates failed");
	
	switch (error_type) {
	case UpdateChecker::ErrorType::MTOOL_FAILED_TO_START:
		message_box->setText("Couldn't start the maintenance tool.");
		break;
		
	case UpdateChecker::ErrorType::MTOOL_CRASHED:
		message_box->setText("The maintenance tool crashed.");
		break;
		
	case UpdateChecker::ErrorType::MTOOL_TIMEDOUT:
		message_box->setText("The maintenance tool timedout.");
		break;
		
	case UpdateChecker::ErrorType::MTOOL_WRITEERROR:
		message_box->setText("The maintenance tool had trouble writing stuff.");
		break;
		
	case UpdateChecker::ErrorType::MTOOL_READERROR:
		message_box->setText("The maintenance tool had trouble reading stuff.");
		break;
		
	case UpdateChecker::ErrorType::REPO_NETWORK_ERROR:
		message_box->setText("Couldn't retrive information from the update server.");
		break;
		
	default:
		message_box->setText("Unknown error.");
		break;
	}
	
	message_box->exec();
	accept();
}

void UpdaterDialog::disconnectUpdateChecker()
{
	disconnect(update_checker, nullptr, this, nullptr);
}