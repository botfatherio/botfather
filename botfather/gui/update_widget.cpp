#include "update_widget.h"
#include "ui_update_widget.h"
#include <QMessageBox>
#include <QApplication>
#include <QProcess>
#include <QDebug>

#include <QThread>

UpdateWidget::UpdateWidget(QWidget *parent) : QWidget(parent), ui(new Ui::UpdateWidget)
{
	ui->setupUi(this);
	adjustSize();
	setFixedSize(400, height());

	update_checker_thread = new QThread(this);
	update_checker = new UpdateChecker();
	update_checker->moveToThread(update_checker_thread);
	connect(update_checker_thread, SIGNAL(started()), update_checker, SLOT(checkForUpdates()));

	connect(ui->cancel_button, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui->cancel_button, SIGNAL(clicked()), update_checker, SLOT(cancelUpdateCheck()));
	connect(ui->cancel_button, SIGNAL(clicked()), this, SIGNAL(finished()));
	connect(update_checker, SIGNAL(noUpdatesAvailable()), this, SLOT(close()));
	connect(update_checker, SIGNAL(noUpdatesAvailable()), this, SIGNAL(finished()));
	connect(update_checker, SIGNAL(updatesAvailable()), this, SLOT(close()));
	connect(update_checker, SIGNAL(updatesAvailable()), this, SLOT(informAboutUpdate()));
	connect(update_checker, SIGNAL(updateCheckFailed(UpdateChecker::ErrorType)), this, SLOT(close()));
	connect(update_checker, SIGNAL(updateCheckFailed(UpdateChecker::ErrorType)), this, SLOT(informAboutError(UpdateChecker::ErrorType)));
}

UpdateWidget::~UpdateWidget()
{
	delete ui;
}

void UpdateWidget::checkForUpdates()
{
	update_checker_thread->start();
}

void UpdateWidget::informAboutUpdate()
{
	QMessageBox::StandardButton answer;
	answer = QMessageBox::question(this, "Updates Available!", "Updates available. Update now?", QMessageBox::Yes|QMessageBox::No);
	if (answer == QMessageBox::Yes) {
		runUpdater();
		QApplication::quit();
	}
	emit finished();
}

void UpdateWidget::informAboutError(UpdateChecker::ErrorType error_type)
{
	QMessageBox message_box;
	message_box.setIcon(QMessageBox::Warning);
	message_box.setText("Checking for updates failed.");
	
	switch (error_type) {
	case UpdateChecker::ErrorType::MTOOL_FAILED_TO_START:
		message_box.setInformativeText("Couldn't start the maintenance tool.");
		break;
		
	case UpdateChecker::ErrorType::MTOOL_CRASHED:
		message_box.setInformativeText("The maintenance tool crashed.");
		break;
		
	case UpdateChecker::ErrorType::MTOOL_TIMEDOUT:
		message_box.setInformativeText("The maintenance tool timedout.");
		break;
		
	case UpdateChecker::ErrorType::MTOOL_WRITEERROR:
		message_box.setInformativeText("The maintenance tool had trouble writing stuff.");
		break;
		
	case UpdateChecker::ErrorType::MTOOL_READERROR:
		message_box.setInformativeText("The maintenance tool had trouble reading stuff.");
		break;
		
	case UpdateChecker::ErrorType::REPO_NETWORK_ERROR:
		message_box.setInformativeText("Couldn't retrive information from the update server.");
		break;
		
	default:
		message_box.setInformativeText("Unknown error.");
		break;
	}
	
	message_box.exec();
	emit finished();
}

void UpdateWidget::runUpdater()
{
	QStringList args("--updater");
	QProcess::startDetached(UpdateChecker::maintainancetoolPath(), args);
}