#include "control_window.h"
#include <QMessageBox>
#include <QUrl>
#include <QDebug>
#include <QDesktopServices>
#include <QCloseEvent>
#include "ui_control_window.h"
#include "shared/constants.h"

ControlWindow::ControlWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ControlWindow)
{
	ui->setupUi(this);
}

ControlWindow::~ControlWindow()
{
	delete ui;
}

void ControlWindow::on_actionStart_triggered()
{
	QString script_path = QFileDialog::getOpenFileName(
		this,
		tr("Run Bot Script"),
		"",
		tr("Bot Script Files (*.js)"),
		Q_NULLPTR,
		// Triggering the file dialog more than once using the native dialog made the program get stuck.
		QFileDialog::DontUseNativeDialog
	);
	
	if (script_path.isEmpty()) {
		// No script selected. Reset ui modifications and tell the user using the status bar.
		this->ui->statusBar->showMessage("No script selected.", 5000);
		return;
	}
	
	// Disable start and stop button while we setup the bot.
	this->ui->actionStart->setEnabled(false);
	this->ui->actionStop->setEnabled(false);
	
	// Create thread seperate from the main thread that can be terminated if necessary.
	this->bot_thread = new QThread;
	
	// Create an new bot instance with the given script path and move it into the thread.
	Bot* bot = new Bot(script_path);
	bot->moveToThread(this->bot_thread);
	
	// Make the bot start when the thread starts and delete the thread object when
	// the thread finished.
	connect(this->bot_thread, &QThread::started, bot, &Bot::runScript);
	connect(this->bot_thread, &QThread::finished, bot, &QObject::deleteLater);
	
	// Make ControlWindow methods be called when the bot started and stopped.
	connect(bot, &Bot::started, this, &ControlWindow::bot_started);
	connect(bot, &Bot::stopped, this, &ControlWindow::bot_stopped);
	
	// Enable logging visible for the user
	connect(bot, &Bot::message, &this->log_dialog, &LogDialog::appendMessage);
	
	// Start the bot thread and thus the bot.
	this->bot_thread->start();
}

void ControlWindow::bot_started()
{
	// The bot started, enable the stop button.
	this->ui->actionStop->setEnabled(true);
}

void ControlWindow::on_actionStop_triggered()
{
	this->ui->actionStart->setEnabled(false);
	this->ui->actionStop->setEnabled(false);
	this->ui->mainToolBar->insertAction(this->ui->actionStop, this->ui->actionKill);
	this->ui->mainToolBar->removeAction(this->ui->actionStop);
	
	// TODO: Request interruption of the bot script.
}

void ControlWindow::on_actionKill_triggered()
{
	qDebug() << "Action kill triggered";
	this->bot_thread->terminate();
}

void ControlWindow::bot_stopped(bool without_errors)
{
	this->ui->actionStart->setEnabled(true);
	this->ui->mainToolBar->insertAction(this->ui->actionKill, this->ui->actionStop);
	this->ui->mainToolBar->removeAction(this->ui->actionKill);
	
	if (!without_errors) {
		// Encourage the user to check the logs because errors occurred executing the script.
		QMessageBox::warning(
			this,
			"Errors occurred",
			"Script executiong wasn't successfully. Errors occurrred. Please check the log."
		);
	}
}

void ControlWindow::on_actionLog_triggered()
{
	this->log_dialog.show();
}

void ControlWindow::on_actionSettings_triggered()
{
	// When executing (opening) the config dialog the control window can't be clicked.
	this->config_dialog.exec();
}

void ControlWindow::on_actionScripts_triggered()
{
	QDesktopServices::openUrl(QUrl("https://botfather.io/scripts/"));
}

void ControlWindow::on_actionSupport_triggered()
{
	QDesktopServices::openUrl(QUrl("https://botfather.io/support/"));
}

void ControlWindow::on_actionAbout_triggered()
{
	QMessageBox::about(this, "About Botfather", "Visit https://botfather.io/ for more information.");
}