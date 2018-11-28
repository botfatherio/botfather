#include "control_window.h"
#include "ui_control_window.h"
#include <QMessageBox>
#include <QUrl>
#include <QDebug>
#include <QDesktopServices>
#include <QCloseEvent>
#include <QSettings>
#include <QThread>
#include <QDateTime>
#include <QProcess>
#include "config_dialog.h"
#include "browser_window.h"
#include "android_dialog.h"
#include "auth_dialog.h"
#include "scriptmanager/scriptmanagerdialog.h"
#include "../settings.h"
#include "../tools/mtoolwrapper.h"

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

	ConfigDialog* config_dialog = new ConfigDialog(this);
	AndroidDialog* android_dialog = new AndroidDialog(this);
	browser_window = new BrowserWindow(); // Don't give it a parent, otherwise it's blocking the control window on microsoft windows
	media_player = new QMediaPlayer(this);

	stop_hotkey = new QHotkey();
	connect(stop_hotkey, &QHotkey::activated, this, &ControlWindow::stopBot);
	updateHotkeys();

	adjustLimitations(false);

	AuthDialog* auth_dialog = new AuthDialog(this);
	connect(auth_dialog, &AuthDialog::authenticated, this, &ControlWindow::onLoggedIn);
	connect(auth_dialog, &AuthDialog::triedAutoLogin, [&]() {
		ui->menuAccount->setEnabled(true); // Was initialy disabled in the .ui file
	});
	QTimer::singleShot(0, auth_dialog, &AuthDialog::tryAutoLogin); // do not block the constructor

	connect(ui->actionShowLogin, &QAction::triggered, auth_dialog, &AuthDialog::exec);
	connect(ui->actionLogout, &QAction::triggered, this, &ControlWindow::onLogout);

	connect(ui->actionSettings, &QAction::triggered, config_dialog, &ConfigDialog::exec);
	connect(ui->actionAndroid, &QAction::triggered, android_dialog, &AndroidDialog::exec);
    connect(ui->actionBrowser, &QAction::triggered, browser_window, &BrowserWindow::show);

	connect(config_dialog, &ConfigDialog::configLoaded, this, &ControlWindow::updateHotkeys);
	connect(config_dialog, &ConfigDialog::configSaved, this, &ControlWindow::updateHotkeys);

	connect(ui->actionStart, &QAction::triggered, this, &ControlWindow::onStartClicked);
	connect(ui->actionStop, &QAction::triggered, this, &ControlWindow::stopBot);

	// File Menu Actions
	MtoolWrapper* mtool = new MtoolWrapper(this);
	connect(ui->menuFile, &QMenu::aboutToShow, mtool, &MtoolWrapper::findExecutable);
	connect(mtool, &MtoolWrapper::executableFound, ui->actionCheckForUpdates, &QAction::setEnabled);
	connect(ui->actionCheckForUpdates, &QAction::triggered, mtool, &MtoolWrapper::startDetachedUpdater);
	connect(mtool, &MtoolWrapper::startedDetached, this, &ControlWindow::close);
	connect(ui->actionExit, &QAction::triggered, this, &ControlWindow::close);
}

ControlWindow::~ControlWindow()
{
	delete ui;
	delete stop_hotkey;
    delete browser_window; // Has no parent
}

void ControlWindow::adjustLimitations(bool is_premium)
{
	if (is_premium)
	{
		ui->actionStatus->setText("Status: 1 bot, no time limit");
		disconnect(&runtimer, &QTimer::timeout, this, &ControlWindow::runtimerTimedOut);
		disconnect(&runtimer, &QTimer::timeout, this, &ControlWindow::stopBot);
	}
	else
	{
		ui->actionStatus->setText("Status: 1 bot, stops after 90 minutes");
		connect(&runtimer, &QTimer::timeout, this, &ControlWindow::runtimerTimedOut);
		connect(&runtimer, &QTimer::timeout, this, &ControlWindow::stopBot);
	}
}

void ControlWindow::runtimerTimedOut()
{
	QString msg1("The bot stopped after 90 minutes, because you were not logged in.");
	QString msg2("Go to Account > Login to prevent the bot from stopping after 90 minutes.");
	appendMessage(msg1, Bot::LogSource::System);
	appendMessage(msg2, Bot::LogSource::System);
}

void ControlWindow::onLoggedIn(int curtime, int premend)
{
	adjustLimitations(premend > curtime);
	ui->actionLogout->setVisible(true);
	ui->actionShowLogin->setVisible(false);
}

void ControlWindow::onLogout()
{
	adjustLimitations(false);
	ui->actionShowLogin->setVisible(true);
	ui->actionLogout->setVisible(false);
}

void ControlWindow::onStartClicked()
{
	ScriptManagerDialog dialog(this);
	connect(&dialog, SIGNAL(scriptExecRequest(QString)), this, SLOT(scriptExecRequested(QString)));
	dialog.exec();
}

void ControlWindow::scriptExecRequested(const QString &script_path)
{
	if (bot && bot->isRunning())
	{
		// There is already a script running
		QMessageBox::information(this, "Can't run this script", "There is already a script running. Stop it first, before running other scripts.");
		return;
	}

	checkPermissions();
	startBot(script_path);
}

void ControlWindow::checkPermissions()
{
#ifdef Q_OS_LINUX
	// On linux the desktop api needs permission to write to /dev/uinput to generate
	// authentic not ignored input events. On Ubuntu the file is writable (660). On other
	// systems it might not be, so we must check that.

	QFileInfo fi("/dev/uinput");
	if (!fi.isWritable()) {
		QString pkexec_path = QStandardPaths::findExecutable("pkexec", {"/usr/bin/"});

		QMessageBox *box = new QMessageBox;
		box->setWindowTitle("Permissions required");
		box->setWindowIcon(QIcon(":/gui/logo_v2.png"));
		box->setIcon(QMessageBox::Information);
		box->setDefaultButton(QMessageBox::Ok);

		box->setDetailedText(
			"Botfathers Desktop API uses the uinput linux kernel module to generate keyboard and mouse input events. "
			"Thus botfather requires write permission on the '/dev/uinput' file.\n"
			"On some distributions that file is writable by default. On your machine it's currently not writable. "
			"The command 'chmod 662 /dev/uinput' makes it writable.\n"
			"When 'pkexec' is installed on your system botfather can execute the command for you. "
			"Otherwise you have to execute it manually from a command line.\n"
			"If you don't want to run scripts controlling your desktop anyway, you don't have to make that file writable."
		);

		if (pkexec_path.isEmpty())
		{
			box->setText(
				"Botfather requires write permissions on '/dev/uinput' to fake keyboard and mouse input events.\n"
				"Run 'sudo chmod 662 /dev/uinput' to grant the required permissions."
			);
			box->setStandardButtons(QMessageBox::Ok);
		}
		else
		{
			box->setText(
				"Botfather requires write permissions on '/dev/uinput' to fake keyboard and mouse input events.\n"
				"Click 'Ok' to grant the required permissions."
			);
			box->setStandardButtons(QMessageBox::Ignore | QMessageBox::Ok);
		}

		if (box->exec() == QMessageBox::Ok && !pkexec_path.isEmpty()) {
			QProcess *p = new QProcess;
			p->start(pkexec_path, {"chmod", "662", "/dev/uinput"});
			p->waitForFinished();
		}
	}
#endif
}

void ControlWindow::startBot(const QString &script_path)
{
	ui->actionStart->setEnabled(false);
	ui->actionStop->setEnabled(false);
	ui->actionLogout->setEnabled(false);

	bot_thread = new QThread;
	bot = new Bot(script_path);
	bot->moveToThread(bot_thread);

	connect(bot, &Bot::started, this, &ControlWindow::botStarted);
	connect(bot, &Bot::stopped, this, &ControlWindow::botStopped);
	connect(bot, &Bot::log, this, &ControlWindow::appendMessage);

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

	// NOTE: The following edge cases have the same effect:
	// A. Stopping the bot in the same moment the timer times out
	// B. Hitting the stop button twice in the same moment
	// => The runtimer doesn't introduce a new "bug".
	runtimer.start(about_90_minutes_in_ms);
}

void ControlWindow::stopBot()
{
	ui->actionStart->setEnabled(false);
	ui->actionStop->setEnabled(false);

	runtimer.stop();
	bot->stop();
}

void ControlWindow::botStopped(bool without_errors)
{
	stop_hotkey->setRegistered(false);
	ui->actionStart->setEnabled(true);
	ui->actionStop->setEnabled(false);
	ui->actionLogout->setEnabled(true);

	stopWavSound();
	
	if (!without_errors) {
		QMessageBox::warning(
			this,
			"Errors occurred",
			"Script execution wasn't successful. Errors occurrred. Please check the log."
		);
	}
}

void ControlWindow::appendMessage(const QString &message, const Bot::LogSource &source)
{
	QString time(QDateTime::currentDateTime().toString("HH:mm:ss"));
	QString color, source_name;

	QSettings settings;
	if (source == Bot::LogSource::Debug && !settings.value(general::options::DEVMODE, general::fallback::DEVMODE).toBool())
	{
		// Do not show debug message when not in debug mode
		return;
	}

	switch (source)
	{
	case Bot::LogSource::System:
		color = "#209cee";
		source_name = "system";
		break;
	case Bot::LogSource::Error:
		color = "#ff3860";
		source_name = "error";
		break;
	case Bot::LogSource::Script:
		color = "#4a4a4a";
		source_name = "script";
		break;
	case Bot::LogSource::Debug:
		color = "#85732d"; // #ffdd57
		source_name = "debug";
		break;
	}

	QString text = QString("<span style='color:%1'>%2 | %3 &gt; %4</span>").arg(color).arg(time).arg(source_name).arg(message);
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

void ControlWindow::on_actionAbout_triggered()
{
	QString text = QString(
		"<html><body>"
		"<p><b>Botfather</b> is a universal automation framework for creating and running bots playing online games, android apps and more.</p>"
		"<p><a href='https://botfather.io/'>Website</a> | <a href='https://botfather.io/docs/legal/privacy/'>Privacy</a> | <a href='https://botfather.io/docs/legal/tos/'>EULA</a> | Version: %1</p>"
		"<b>Software used by botfather:</b>"
		"<p>Those beautiful application icons are made by Icons8.<br>"
		"<a href='https://icons8.com/'>website</a> | <a href='https://icons8.com/license/'>license</a></p>"
		"<p>Chromium Embedded Framework. Copyright (c) 2008-2014 Marshall A. Greenblatt.<br>"
		"<a href='https://bitbucket.org/chromiumembedded/cef'>website</a> | <a href='https://bitbucket.org/chromiumembedded/cef/src/master/LICENSE.txt'>license</a></p>"
		"<p>OpenSSL. Copyright © 1998-2018 The OpenSSL Project.<br>"
		"<a href='https://www.openssl.org/'>website</a> | <a href='https://www.openssl.org/source/license.html'>license</a></p>"
		"<p>Libgit2. Copyright (C) the libgit2 contributors.<br>"
		"<a href='https://libgit2.org/'>website</a> | <a href='https://github.com/libgit2/libgit2/blob/master/COPYING'>license</a></p>"
		"</body></html>"
	).arg(QCoreApplication::applicationVersion());

	QMessageBox::about(this, "About Botfather", text);
}

void ControlWindow::on_actionAboutQt_triggered()
{
	QMessageBox::aboutQt(this);
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
	// NOTE: Nope, there is no 'aboutToClose' signal
	browser_window->close();

	// NOTE: Sometimes after atleast using the script file chooser dialog the application didn't quit after
	// closing the main window. Calling QCoreApplication::quit fixes this issue. Consider this a workaround(?)
	QCoreApplication::quit();

	QMainWindow::closeEvent(event);
}
