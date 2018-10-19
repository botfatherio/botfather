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

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
#include <QRandomGenerator>
#define GenerateRandomIntInRange(MIN, MAX) QRandomGenerator::global()->bounded(MIN, MAX);
#else
#define ImageSizeInBytes(qimage) static_cast<int>(qimage.sizeInBytes())
#define GenerateRandomIntInRange(MIN, MAX) qrand() % ((MAX + 1) - MIN) + MIN
#endif

ControlWindow::ControlWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::ControlWindow)
{
	ui->setupUi(this);

	config_dialog = new ConfigDialog(this);
    browser_window = new BrowserWindow(); // Don't give it a parent, otherwise it's blocking the control window on microsoft windows
	android_dialog = new AndroidDialog(this);
	media_player = new QMediaPlayer(this);
	
	stop_hotkey = new QHotkey();
	connect(stop_hotkey, &QHotkey::activated, this, &ControlWindow::stopBot);
	updateHotkeys();

	connect(ui->actionSettings, &QAction::triggered, config_dialog, &ConfigDialog::exec);
	connect(ui->actionAndroid, &QAction::triggered, android_dialog, &AndroidDialog::exec);
    connect(ui->actionBrowser, &QAction::triggered, browser_window, &BrowserWindow::show);

	connect(config_dialog, &ConfigDialog::configLoaded, this, &ControlWindow::updateHotkeys);
	connect(config_dialog, &ConfigDialog::configSaved, this, &ControlWindow::updateHotkeys);

	connect(ui->actionStart, &QAction::triggered, this, &ControlWindow::startBot);
	connect(ui->actionStop, &QAction::triggered, this, &ControlWindow::stopBot);
}

ControlWindow::~ControlWindow()
{
	delete ui;
	delete stop_hotkey;
    delete browser_window; // Has no parent
}

void ControlWindow::applyRemoteApiInfo(int curtime, int premend, bool stable)
{	
	// TODO: remove stable parameter
}

void ControlWindow::startBot()
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

	bot_thread = new QThread;
	bot = new Bot(script_path);
	bot->moveToThread(bot_thread);

	connect(bot, &Bot::started, this, &ControlWindow::botStarted);
	connect(bot, &Bot::stopped, this, &ControlWindow::botStopped);
	connect(bot, &Bot::message, this, &ControlWindow::appendMessage);

	// QSound only works in the main thread, thats why we have to the control window to play
	// the desired sound whenever to bot wants us to do so.
	connect(bot, &Bot::playWavSound, this, &ControlWindow::playWavSound);
	connect(bot, &Bot::stopWavSound, this, &ControlWindow::stopWavSound);

	connect(bot_thread, &QThread::started, bot, &Bot::runScript);
	connect(bot, &Bot::stopped, bot_thread, &QThread::quit);
	connect(bot, &Bot::stopped, bot, &QObject::deleteLater);
	connect(bot_thread, &QThread::finished, bot_thread, &QObject::deleteLater);

	bot_thread->start();
}

void ControlWindow::botStarted()
{
	ui->actionStop->setEnabled(true);
	stop_hotkey->setRegistered(true);

	// Stop the bot after a certain time for non premium accounts.
	int about_90_minutes_in_ms = GenerateRandomIntInRange(1000 * 60 * 89, 1000 * 60 * 91);
	QTimer::singleShot(about_90_minutes_in_ms, this, &ControlWindow::stopBot);
}

void ControlWindow::stopBot()
{
	ui->actionStart->setEnabled(false);
	ui->actionStop->setEnabled(false);
	bot->stop();
}

void ControlWindow::botStopped(bool without_errors)
{
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

void ControlWindow::appendMessage(QString message, bool from_botfather, bool error)
{
	QString color(error ? "#ff3860" : (from_botfather ? "#209cee" : "#4a4a4a"));
	QString time(QDateTime::currentDateTime().toString("HH:mm:ss"));
	QString user(from_botfather ? "system" : "script");
	QString text = QString("<span style='color:%1'>%2 | %3 &gt; %4</span>").arg(color).arg(time).arg(user).arg(message);
	ui->log_text->append(text);
}

void ControlWindow::on_save_button_clicked()
{
	QString log = this->ui->log_text->toPlainText();

	QString filename = QFileDialog::getSaveFileName(
		this,
		tr("Save Logfile"),
		QDir::homePath(),
		tr("Text files (*.txt *.log)"),
		Q_NULLPTR,
		// Triggering the file dialog more than once using the native dialog made the program get stuck.
		QFileDialog::DontUseNativeDialog | QFileDialog::DontUseCustomDirectoryIcons
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

void ControlWindow::on_actionPremiumPlans_triggered()
{
	// TODO: delete this action
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
	stopWavSound();
	media_player->setMedia(QUrl::fromLocalFile(path_to_wav_file));
	media_player->play();
}

void ControlWindow::stopWavSound()
{
	media_player->stop();
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

void ControlWindow::closeEvent(QCloseEvent* event)
{
    browser_window->close();
}
