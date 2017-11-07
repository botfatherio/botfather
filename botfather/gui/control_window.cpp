#include "control_window.h"
#include <QMessageBox>
#include <QUrl>
#include <QDebug>
#include <QDesktopServices>
#include <QCloseEvent>
#include "ui_control_window.h"
#include "browser/browser.h"
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

void ControlWindow::on_actionHome_triggered()
{
	Browser::loadUrl(settings.value("BROWSER_STARTPAGE", constants::BROWSER_STARTPAGE).toString());
}

void ControlWindow::on_actionReload_triggered()
{
	Browser::refreshPage();
}

void ControlWindow::on_actionStart_triggered()
{
	this->ui->actionStart->setEnabled(false);
	this->ui->actionStop->setEnabled(false);
	
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
		this->ui->actionStart->setEnabled(true);
		this->ui->actionStop->setEnabled(false);
		return;
	}
	
	// Prepare a bot instance to be run in a thread seperated from the main thread.
	this->bot_thread = new QThread;
	Bot* bot = new Bot(script_path);
	bot->moveToThread(this->bot_thread);
	
	// Make the bot start when the thread starts and delete the thread object when
	// the thread finished.
	connect(this->bot_thread, &QThread::started, bot, &Bot::runScript);
	connect(this->bot_thread, &QThread::finished, bot, &QObject::deleteLater);
	
	// Make ControlWindow methods be called when the bot started and stopped.
	connect(bot, &Bot::started, this, &ControlWindow::bot_started);	
	connect(bot, &Bot::stopped, this, &ControlWindow::bot_stopped);
	
	// Start the bot thread and thus the bot.
	this->bot_thread->start();
}

void ControlWindow::bot_started()
{
	this->ui->actionStop->setEnabled(true);
}

void ControlWindow::on_actionStop_triggered()
{
	this->ui->actionStart->setEnabled(false);
	this->ui->actionStop->setEnabled(false);
	
	// Request interruption of the bots main loop. As a result the bot will stop.
	this->bot_thread->requestInterruption();
}

void ControlWindow::bot_stopped()
{
	this->ui->actionStart->setEnabled(true);
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