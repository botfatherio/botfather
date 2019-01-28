#include "botwidget.h"
#include "ui_botwidget.h"
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QProcess>
#include <QRegularExpression>
#include <QStandardPaths>

BotWidget::BotWidget(Bot *bot, QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::BotWidget)
	, m_bot(bot)
{
	ui->setupUi(this);
	layout()->setMargin(0);
	updateBotName(bot->name());

	media_player = new QMediaPlayer(this);
	bot_settings = new QSettings(bot->settingsPath(), QSettings::IniFormat);

	connect(ui->save_log, &QPushButton::clicked, this, &BotWidget::saveLogToFile);
	connect(ui->clear_log, &QPushButton::clicked, ui->log, &QTextEdit::clear);

	connect(bot, &Bot::nameChanged, this, &BotWidget::updateBotName);
	connect(bot, &Bot::log, this, &BotWidget::log);
	connect(bot, &Bot::audioPlaybackRequested, this, &BotWidget::playWavSound);
	connect(bot, &Bot::audioStopRequested, this, &BotWidget::stopWavSound);

	connect(bot, &Bot::stopped, &runtimer, &QTimer::stop);
	runtimer.callOnTimeout(this, &BotWidget::runtimerTimedOut);
	runtimer.callOnTimeout(this, &BotWidget::tryBotStop);

	// Storing the bot setting when the application is about to quit delays the quit noticeable
	connect(ui->debug_mode, &QCheckBox::clicked, this, &BotWidget::saveBotSettings);
	if (bot->isValid())
	{
		loadBotSettings();
	}
}

BotWidget::~BotWidget()
{
	delete ui;
}

void BotWidget::updateBotName(const QString &new_bot_name)
{
	ui->label->setText(new_bot_name);
}

void BotWidget::loadBotSettings()
{
	ui->debug_mode->setChecked(bot_settings->value("debug_mode", false).toBool());
}

void BotWidget::saveBotSettings()
{
	bot_settings->setValue("debug_mode", ui->debug_mode->isChecked());
}

void BotWidget::tryBotStart(int runtime_in_secs)
{
	if (m_bot->isRunning())
	{
		qDebug() << "Can't start already running bot";
		return;
	}

	checkPermissions(m_bot->scriptPath());

	if (runtime_in_secs > 0)
	{
		runtimer.setInterval(runtime_in_secs * 1000);
		int runtime_in_minutes = qRound(runtimer.interval() / 1000 / 60.0);
		QString limitation_msg = QString("The bot will run for %0 minutes, login first to avoid this limitation.").arg(runtime_in_minutes);
		log(limitation_msg, Engine::LogSource::System);
		runtimer.start();
	}

	m_bot->start();
}

void BotWidget::tryBotStop()
{
	if (!m_bot->isRunning())
	{
		qDebug() << "Can't stop not running bot";
		return;
	}

	runtimer.stop();
	m_bot->stop();
}

void BotWidget::runtimerTimedOut()
{
	int runtime_in_minutes = qRound(runtimer.interval() / 1000 / 60.0);
	QString msg1 = QString("The bot stopped after %0 minutes, because you were not logged in.").arg(runtime_in_minutes);
	QString msg2 = QString("Go to Account > Login to prevent the bot from stopping after %0 minutes.").arg(runtime_in_minutes);
	log(msg1, Engine::LogSource::System);
	log(msg2, Engine::LogSource::System);
}

void BotWidget::log(const QString &message, const Engine::LogSource &source)
{
	QString time(QDateTime::currentDateTime().toString("HH:mm:ss"));
	QString color, source_name;

	if (source == Engine::LogSource::Debug && !bot_settings->value("debug_mode").toBool())
	{
		// Do not show debug message when not in debug mode
		return;
	}

	switch (source)
	{
	case Engine::LogSource::System:
		color = "#209cee";
		source_name = "system";
		break;
	case Engine::LogSource::Error:
		color = "#ff3860";
		source_name = "error";
		break;
	case Engine::LogSource::Script:
		color = "#4a4a4a";
		source_name = "script";
		break;
	case Engine::LogSource::Debug:
		color = "#85732d"; // #ffdd57
		source_name = "debug";
		break;
	}

	QString text = QString("<span style='color:%1'>%2 | %3 &gt; %4</span>").arg(color).arg(time).arg(source_name).arg(message);
	ui->log->append(text);
}

void BotWidget::saveLogToFile()
{
	QString filename = QFileDialog::getSaveFileName(
		this,
		tr("Save Logfile"),
		QDir::homePath(),
		tr("Text files (*.txt *.log)"),
		nullptr
	);

	if (filename.isEmpty()){
		qDebug() << "No logfile selected.";
		return;
	}

	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		qDebug() << "Can't open logfile" << filename;
		return;
	}
	file.write(ui->log->toPlainText().toUtf8());
	file.close();
}

void BotWidget::playWavSound(const QString &path_to_wav_file)
{
	// QMediaPlayer does only work in the main thread.
	stopWavSound();
	media_player->setMedia(QUrl::fromLocalFile(path_to_wav_file));
	media_player->play();
}

void BotWidget::stopWavSound()
{
	media_player->stop();
}

void BotWidget::checkPermissions(const QString &script_path)
{
#ifdef Q_OS_LINUX
	// On linux the desktop api needs permission to write to /dev/uinput to generate
	// authentic not ignored input events. On Ubuntu the file is writable (660). On other
	// systems it might not be, so we must check that.

	if (QFileInfo("/dev/uinput").isWritable())
	{
		// We already have the required permissions to fake mouse and keyboard input on linux
		return;
	}

	QFile script_file(script_path);
	if (!script_file.open(QIODevice::ReadOnly))
	{
		return;
	}

	QString script_text = QString(script_file.readAll());
	script_file.close();

	if (!script_text.contains(QRegularExpression("Desktop([\\n \\t\\r]*)\\.[\\w]+([\\n \\t\\r]*)\\(")))
	{
		// The script does not make use of the Desktop API which we require the permissions for
		return;
	}

	QMessageBox *box = new QMessageBox;
	box->setWindowTitle("Permissions required");
	box->setWindowIcon(QIcon(":/gui/logo_v2.png"));
	box->setIcon(QMessageBox::Information);
	box->setStandardButtons(QMessageBox::Ignore | QMessageBox::Ok);
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

	QString pkexec_path = QStandardPaths::findExecutable("pkexec", {"/usr/bin/"});

	if (pkexec_path.isEmpty())
	{
		box->setText(
			"Botfather requires write permissions on '/dev/uinput' to fake keyboard and mouse input events.\n"
			"Run 'sudo chmod 662 /dev/uinput' to grant the required permissions."
		);
	}
	else
	{
		box->setText(
			"Botfather requires write permissions on '/dev/uinput' to fake keyboard and mouse input events.\n"
			"Click 'Ok' to grant the required permissions."
		);
	}

	if (box->exec() == QMessageBox::Ok && !pkexec_path.isEmpty()) {
		QProcess *p = new QProcess;
		p->start(pkexec_path, {"chmod", "662", "/dev/uinput"});
		p->waitForFinished();
	}
#endif
}
