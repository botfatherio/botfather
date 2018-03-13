#include "control_window.h"
#include "ui_control_window.h"
#include <QMessageBox>
#include <QUrl>
#include <QDebug>
#include <QDesktopServices>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QSettings>
#include "config_dialog.h"
#include "browser_window.h"
#include "../shared/constants.h"

ControlWindow::ControlWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::ControlWindow)
{
	ui->setupUi(this);

	// Make the main window parent of the dialogs to make them close when the main win is.
	this->browser_window = new BrowserWindow(this);
}

ControlWindow::~ControlWindow()
{
	delete ui;
}

void ControlWindow::open(bool stable, bool trial)
{
	this->trial = trial;
	if (stable){
		this->setWindowTitle(this->windowTitle() + " - Stable");
	}
	if (trial) {
		this->setWindowTitle(this->windowTitle() + " - Trial");
	}
	this->show();
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
		// No script selected. Reset ui modifications and tell the user.
		this->appendMessage("No script selected.", true);
		return;
	}
	
	// Disable start and stop button while we setup the bot.
	this->ui->actionStart->setEnabled(false);
	this->ui->actionStop->setEnabled(false);
	
	// Create thread seperate from the main thread that can be terminated if necessary.
	this->bot_thread = new BotThread;
	
	// Create an new bot instance with the given script path and move it into the thread.
	Bot* bot = new Bot(this->bot_thread, script_path);
	bot->moveToThread(this->bot_thread);

	// Make the bot start when the thread starts and delete the thread object when
	// the thread finished.
	connect(this->bot_thread, &QThread::started, bot, &Bot::runScript);
	connect(this->bot_thread, &QThread::finished, bot, &QObject::deleteLater);
	
	// Make ControlWindow methods be called when the bot started and stopped.
	connect(bot, &Bot::started, this, &ControlWindow::bot_started);
	connect(bot, &Bot::stopped, this, &ControlWindow::bot_stopped);
	
	// Enable logging visible for the user
	connect(bot, &Bot::message, this, &ControlWindow::appendMessage);
	
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
	this->ui->actionStop->setVisible(false);
	this->ui->actionKill->setVisible(true);	
	
	// This causes HelperAPI::shouldRun() to return false.
	this->bot_thread->requestInterruption();
}

void ControlWindow::on_actionKill_triggered()
{
	this->bot_thread->terminate();
	this->appendMessage("The script has been killed. RIP.", true);
	this->bot_stopped(true);
}

void ControlWindow::bot_stopped(bool without_errors)
{
	this->ui->actionStop->setVisible(true);
	this->ui->actionKill->setVisible(false);
	this->ui->actionStart->setEnabled(true);
	this->ui->actionStop->setEnabled(false);
		
	if (!without_errors) {
		// Encourage the user to check the logs because errors occurred executing the script.
		QMessageBox::warning(
			this,
			"Errors occurred",
			"Script executiong wasn't successfully. Errors occurrred. Please check the log."
		);
	}
}

void ControlWindow::on_actionSettings_triggered()
{
	// Initiate a new config dialog making it load all current settings and
	// execute it, so that the control window can't be clicked while it's open.
	ConfigDialog *config_dialog = new ConfigDialog(this);
	config_dialog->exec();
}

void ControlWindow::on_actionBrowser_triggered()
{
	this->browser_window->show();
}

void ControlWindow::on_actionScripts_triggered()
{
	QDesktopServices::openUrl(QUrl("https://botfather.io/scripts/"));
}

void ControlWindow::on_actionAbout_triggered()
{
	QMessageBox::about(
		this,
		"About Botfather",
		"<html><body>"
		"<p>Botfather.io is framework and engine for creating and running bots playing online games, apps and more.</p>"
		"<p>You can bot any game, website or software you want. Just pick the right engine and get or create a script for it.</p>"
		"<p>Visit <a href='https://botfather.io/'>botfather.io</a> for more information.</p>"
		"<p>By the way: Those beautiful icons are from <a href='https://icons8.com/'>Icons8</a></p>"
		"</body></html>"
	);
}

void ControlWindow::on_actionAboutQt_triggered()
{
	QMessageBox::aboutQt(this);
}

void ControlWindow::on_save_button_clicked()
{
	QString log = this->ui->log_text->toPlainText();
	
	QString filename = QFileDialog::getSaveFileName(
		this,
		tr("Save Logfile"),
		"",
		tr("Text files (*.txt *.log)"),
		Q_NULLPTR,
		// Triggering the file dialog more than once using the native dialog made the program get stuck.
		QFileDialog::DontUseNativeDialog
	);
	
	if (filename.isEmpty()){
		qDebug() << "No file selected.";
		return;
	}
	
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		qDebug() << "Can't open logfile" << filename;
		return;
	}
	file.write(log.toUtf8());
	file.close();
}

void ControlWindow::on_clear_button_clicked()
{
	this->ui->log_text->clear();
}

void ControlWindow::appendMessage(QString log_message, bool from_client)
{
	if (from_client){
		this->ui->log_text->append("<strong style='color:#555'>&lt;botfather&gt;</strong> " + log_message);
	} else{
		this->ui->log_text->append("<strong style='color:#555'>&lt;botscript&gt;</strong> " + log_message);
	}
}