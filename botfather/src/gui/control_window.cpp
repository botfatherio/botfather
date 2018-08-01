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
#include "android_dialog.h"
#include "../scripting/general_settings.h"

ControlWindow::ControlWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::ControlWindow)
{
	ui->setupUi(this);

	config_dialog = new ConfigDialog(this);
	browser_window = new BrowserWindow(this);
	android_dialog = new AndroidDialog(this);
	file_dialog = new QFileDialog(this);
	script_sound_effect = new QSoundEffect(this);

	// LOL everything must be inited. When not initializing kill_timer even checking whether it's
	// a nullptr can cause the program zu crash.
	kill_timer = nullptr;
	
	stop_hotkey = new QHotkey();
	kill_hotkey = new QHotkey();
	connect(stop_hotkey, &QHotkey::activated, this, &ControlWindow::on_actionStop_triggered);
	connect(kill_hotkey, &QHotkey::activated, this, &ControlWindow::on_actionKill_triggered);
	updateHotkeys();
	
	// Store the original window title so it can be restored eg after the user logged out.
	original_window_title = windowTitle();
	
	// The kill timer must be stopped if the user manually kills the script or the script terminates after some time.
	// Unfortunately there is no signal emitted when we kill the bot thread thus we must cancel the timer using other
	// signals (start button pressed, kill button pressed).
	connect(ui->actionStart, &QAction::triggered, this, &ControlWindow::stopKillTimer);
	connect(ui->actionKill, &QAction::triggered, this, &ControlWindow::stopKillTimer);
	
	connect(ui->actionSettings, &QAction::triggered, config_dialog, &ConfigDialog::exec);
	connect(ui->actionBrowser, &QAction::triggered, browser_window, &BrowserWindow::show);
	connect(ui->actionAndroid, &QAction::triggered, android_dialog, &AndroidDialog::exec);
	
	connect(config_dialog, &ConfigDialog::configLoaded, this, &ControlWindow::updateHotkeys);
	connect(config_dialog, &ConfigDialog::configSaved, this, &ControlWindow::updateHotkeys);
}

ControlWindow::~ControlWindow()
{
	delete ui;
}

void ControlWindow::applyRemoteApiInfo(int curtime, int premend, bool stable)
{	
	Q_UNUSED(stable)
	
	trial = curtime > premend;
	// No longer alter the title. Just leave it be Botfather.
	// This method is still usefull though to make scripts stop when premium finished.
	//this->setWindowTitle(original_window_title + (stable ? " - Stable" : "") + (trial ? " - Trial" : " - Premium"));
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
	this->bot_thread = new BotThread(this);
	
	// Create an new bot instance with the given script path and move it into the thread.
	bot = new Bot(this->bot_thread, script_path);
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
	
	// QSound only works in the main thread, thats why we have to the control window to play
	// the desired sound whenever to bot wants us to do so.
	connect(bot, &Bot::playWavSound, this, &ControlWindow::playWavSound);
	connect(bot, &Bot::stopWavSound, this, &ControlWindow::stopWavSound);
	
	// Start the bot thread and thus the bot.
	this->bot_thread->start();
}

void ControlWindow::bot_started()
{
	// The bot started, enable the stop button.
	this->ui->actionStop->setEnabled(true);
	
	stop_hotkey->setRegistered(true);
	kill_hotkey->setRegistered(true);
}

void ControlWindow::on_actionStop_triggered()
{
	this->ui->actionStart->setEnabled(false);
	this->ui->actionStop->setEnabled(false);
	this->ui->actionStop->setVisible(false);
	this->ui->actionKill->setVisible(true);	
	
	// This causes HelperAPI::shouldRun() to return false.
	this->bot_thread->requestInterruption();
	
	// Kill the script if it doesn't stop in time.
	QSettings s;
	if (s.value(general::options::AUTOKILL, general::fallback::AUTOKILL).toBool()) {
		kill_timer = new QTimer(this);
		kill_timer->setSingleShot(true);
		connect(kill_timer, &QTimer::timeout, this, &ControlWindow::on_actionKill_triggered);
		// Kill timer canceling connection are setup in the constructor.
		kill_timer->start(s.value(general::options::AUTOKILL_PERIOD, general::fallback::AUTOKILL_PERIOD).toInt() * 1000);
	}
}

void ControlWindow::on_actionKill_triggered()
{
	if (bot_thread->isRunning()) {
		bot_thread->terminate();
	}
	this->appendMessage("The script has been killed. RIP.", true);
	this->bot_stopped(true);
	stopKillTimer();
}

void ControlWindow::bot_stopped(bool without_errors)
{
	stop_hotkey->setRegistered(false);
	kill_hotkey->setRegistered(false);
	
	this->ui->actionStop->setVisible(true);
	this->ui->actionKill->setVisible(false);
	this->ui->actionStart->setEnabled(true);
	this->ui->actionStop->setEnabled(false);
	
	// Stop sounds started by the script.
	stopWavSound();
	
	if (!without_errors) {
		QMessageBox::warning(
			this,
			"Errors occurred",
			"Script execution wasn't successful. Errors occurrred. Please check the log."
		);
	}
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
		"<p>Botfather.io is framework and engine for creating and running bots playing online games, android apps and more.</p>"
		"<p>You can bot any game, website or app you want. Just pick the right script or create a new one.</p>"
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

void ControlWindow::appendMessage(QString log_message, bool from_client)
{
	if (from_client){
		this->ui->log_text->append("<strong style='color:#555'>&lt;botfather&gt;</strong> " + log_message);
	} else{
		this->ui->log_text->append("<strong style='color:#555'>&lt;botscript&gt;</strong> " + log_message);
	}
}

void ControlWindow::on_actionPremiumPlans_triggered()
{
	QDesktopServices::openUrl(QUrl("https://botfather.io/plans/"));
}

void ControlWindow::on_actionLogout_triggered()
{	
	if (bot && bot->isRunning()) {
		QMessageBox::warning(
			this,
			"Can't logout while a script is running",
			"Please stop the running script before logging out. \n"
			"One can't logout while a script is running. Thanks."
		);
		return;
	}
	setWindowTitle(original_window_title);
	emit loggedOut();
}

void ControlWindow::playWavSound(QString path_to_wav_file)
{
	// TODO: Look into alternative ways to run QSound in the main thread.
	// sure controlwindow will always be in the main thread. but this is not
	// obvious, nor does a method like this, not used by the gui, but in a
	// gui class look ugly.
	// TODO: this sound will not stop playing when the scripts stops. We could
	// make QAudio a single instance. everytime this slot is called we stop the audio,
	// set a new source and play it again. when the bot stops (bot_stopped received)
	// we also stop the audio from playing. Disatvantage: only one audio file can be
	// played at the time.
	stopWavSound();
	script_sound_effect->setSource(QUrl::fromLocalFile(path_to_wav_file));
	script_sound_effect->play();
}

void ControlWindow::stopWavSound()
{
	script_sound_effect->stop();
}

void ControlWindow::stopKillTimer()
{
	if (kill_timer && kill_timer->isActive()) {
		kill_timer->stop();
	}
}

void ControlWindow::updateHotkeys()
{
	QSettings s;
	
	stop_hotkey->resetShortcut();
	kill_hotkey->resetShortcut();
	
	stop_hotkey->setShortcut(QKeySequence::fromString(s.value(general::options::STOP_SHORTCUT).toString()));
	kill_hotkey->setShortcut(QKeySequence::fromString(s.value(general::options::KILL_SHORTCUT).toString()));
	
	// Those shortcuts are not registered yet. We only enable them when they are legal to trigger.
	// eg. kill shall only be triggered when there is something to kill, otherwise the program
	// will crash. Same goes for stop.
}