#include "control_window.h"
#include <QMessageBox>
#include <QUrl>
#include <QDebug>
#include <QDesktopServices>
#include <QCloseEvent>
#include <QHBoxLayout>
#include "ui_control_window.h"
#include "log_dialog.h"
#include "config_dialog.h"
#include "../shared/constants.h"
#include "browser_widget.h"

ControlWindow::ControlWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::ControlWindow)
{
	ui->setupUi(this);

	BrowserWidget* browser_widget = new BrowserWidget(this);
	browser_widget->setTextFormat(Qt::RichText);
	browser_widget->setText("<html><head/><body><p style='font-size:16px;'>Hit the Play button to run a bot script.</p></body></html>");

	QHBoxLayout* layout = new QHBoxLayout();
	layout->setMargin(0);
	layout->addWidget(browser_widget);
	ui->centralWidget->setLayout(layout);
	
	// Make the main window parent of the dialogs to make them close when the main win is.
	this->log_dialog = new LogDialog(this);
	this->config_dialog = new ConfigDialog(this);
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
	connect(bot, &Bot::message, this->log_dialog, &LogDialog::appendMessage);
	
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
	this->log_dialog->appendMessage("The script has been killed. RIP.", true);
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

void ControlWindow::on_actionLog_triggered()
{
	this->log_dialog->show();
}

void ControlWindow::on_actionSettings_triggered()
{
	// When executing (opening) the config dialog the control window can't be clicked.
	this->config_dialog->exec();
}

void ControlWindow::on_actionScripts_triggered()
{
	QDesktopServices::openUrl(QUrl("https://botfather.io/scripts/"));
}

void ControlWindow::on_actionAbout_triggered()
{
	QMessageBox::about(this, "About Botfather", "Visit https://botfather.io/ for more information.");
}