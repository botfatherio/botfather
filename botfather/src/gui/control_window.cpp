#include "control_window.h"
#include "ui_control_window.h"
#include <QMessageBox>
#include <QUrl>
#include <QDebug>
#include <QDesktopServices>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QSettings>
#include <QThread>
#include <QDateTime>
#include "config_dialog.h"
#include "browser_window.h"
#include "android_dialog.h"
#include "../engine/bot.h"
#include "../settings.h"

ControlWindow::ControlWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::ControlWindow)
{
	ui->setupUi(this);

	config_dialog = new ConfigDialog(this);
	browser_window = new BrowserWindow(this);
	android_dialog = new AndroidDialog(this);
	script_sound_effect = new QSoundEffect(this);

	// LOL everything must be inited. When not initializing kill_timer even checking whether it's
	// a nullptr can cause the program zu crash.
	bot = nullptr;
	
	stop_hotkey = new QHotkey();
	connect(stop_hotkey, &QHotkey::activated, this, &ControlWindow::on_actionStop_triggered);
	updateHotkeys();

	connect(ui->actionSettings, &QAction::triggered, config_dialog, &ConfigDialog::exec);
	connect(ui->actionBrowser, &QAction::triggered, browser_window, &BrowserWindow::show);
	connect(ui->actionAndroid, &QAction::triggered, android_dialog, &AndroidDialog::exec);
	
	connect(config_dialog, &ConfigDialog::configLoaded, this, &ControlWindow::updateHotkeys);
	connect(config_dialog, &ConfigDialog::configSaved, this, &ControlWindow::updateHotkeys);
}

ControlWindow::~ControlWindow()
{
	delete ui;
	delete bot;
	delete stop_hotkey;
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
	// Increses memory usage by some bytes caching file icons.
	// https://bugreports.qt.io/browse/QTBUG-10651
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
		appendMessage("No script selected.", true);
		return;
	}
	
	// Disable start and stop button while we setup the bot.
	ui->actionStart->setEnabled(false);
	ui->actionStop->setEnabled(false);

	bot_thread = new QThread(this); // deleted later when QThread::finished has been emitted
	bot = new Bot(script_path); // deleted in ControlWindow::bot_stopped
	bot->moveToThread(bot_thread);

	// Make the bot start when the thread starts and delete the thread object when
	// the thread finished.
	connect(bot_thread, &QThread::started, bot, &Bot::runScript);
	connect(bot_thread, &QThread::finished, bot, &QObject::deleteLater);
	
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
	bot_thread->start();
}

void ControlWindow::bot_started()
{
	ui->actionStop->setEnabled(true);
	stop_hotkey->setRegistered(true);
}

void ControlWindow::on_actionStop_triggered()
{
	ui->actionStart->setEnabled(false);
	ui->actionStop->setEnabled(false);
	bot->stop();
}

void ControlWindow::bot_stopped(bool without_errors)
{
	delete bot; // created in ControlWindow::on_actionStart_triggered

	stop_hotkey->setRegistered(false);
	ui->actionStart->setEnabled(true);
	ui->actionStop->setEnabled(false);

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
	QString text = QString(
		"<html><body>"
		"<p><b>Botfather</b> is framework and engine for creating and running bots playing online games, android apps and more.</p>"
		"<p><a href='https://botfather.io/'>Website</a> | <a href='https://botfather.io/docs/legal/privacy/'>Privacy</a> | <a href='https://botfather.io/docs/legal/tos/'>EULA</a> | Version: %1</p>"
		"<b>Software used by botfather:</b>"
		"<p>Those beautiful application icons are by <a href='https://icons8.com/'>website</a> | <a href='https://icons8.com/license/'>license</a></p>"
		"<p>Chromium Embedded Framework. Copyright (c) 2008-2014 Marshall A. Greenblatt. <a href='https://bitbucket.org/chromiumembedded/cef'>website</a> | <a href='https://bitbucket.org/chromiumembedded/cef/src/master/LICENSE.txt'>license</a></p>"
		"<p>OpenSSL. Copyright © 1998-2018 The OpenSSL Project. <a href='https://www.openssl.org/'>website</a> | <a href='https://www.openssl.org/source/license.html'>license</a></p>"
		"</body></html>"
	).arg(QCoreApplication::applicationVersion());

	QMessageBox::about(this, "About Botfather", text);
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

void ControlWindow::appendMessage(QString message, bool from_botfather, bool error)
{
	QString color(error ? "#ff3860" : (from_botfather ? "#209cee" : "#4a4a4a"));
	QString time(QDateTime::currentDateTime().toString("HH:mm:ss"));
	QString user(from_botfather ? "system" : "script");
	QString text = QString("<span style='color:%1'>%2 | %3 &gt; %4</span>").arg(color).arg(time).arg(user).arg(message);
	ui->log_text->append(text);
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

void ControlWindow::updateHotkeys()
{
	QSettings s;
	
	stop_hotkey->resetShortcut();
	stop_hotkey->setShortcut(QKeySequence::fromString(s.value(general::options::STOP_SHORTCUT).toString()));
	
	// Those shortcuts are not registered yet. We only enable them when they are legal to trigger.
	// eg. kill shall only be triggered when there is something to kill, otherwise the program
	// will crash. Same goes for stop.
}
