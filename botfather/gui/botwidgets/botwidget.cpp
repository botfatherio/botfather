#include "botwidget.hpp"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileInfo>
#include <QProcess>
#include <QRegularExpression>
#include <QStandardPaths>

BotWidget::BotWidget(Bot *bot, QSystemTrayIcon *trayicon, QWidget *parent)
	: AbstractBotWidget(bot, trayicon, parent)
{
	m_tab_widget = new QTabWidget(this);
	m_corner_widget = new QLabel(m_tab_widget);
	m_bot_log_widget = new BotLogWidget(bot, this);
	m_bot_updates_widget = new BotUpdatesWidget(bot, this);
	m_bot_browsers_widget = new BotBrowsersWidget(bot, this);
	m_bot_settings_widget = new BotSettingsWidget(bot, this);
	m_bot_settings = new QSettings(bot->settingsPath(), QSettings::IniFormat);
	m_stop_hotkey = new QHotkey();

	setLayout(new QVBoxLayout(this));
	layout()->setMargin(0);
	layout()->addWidget(m_tab_widget);

	m_tab_widget->setCornerWidget(m_corner_widget);
	m_tab_widget->addTab(m_bot_log_widget, "Log");
	setupConfigTab();
	m_tab_widget->addTab(m_bot_browsers_widget, "Browsers");
	m_tab_widget->addTab(m_bot_updates_widget, "Updates");
	m_tab_widget->addTab(m_bot_settings_widget, "Settings");

	if (bot->repo().isEmpty())
	{
		// Disable the updates tab for bots without repo url
		int updates_widget_index = m_tab_widget->indexOf(m_bot_updates_widget);
		m_tab_widget->setTabEnabled(updates_widget_index, false);
	}
	connect(m_bot_updates_widget, &BotUpdatesWidget::botUpdated, this, &BotWidget::setupConfigTab);

	connect(bot, &Bot::nameChanged, this, &BotWidget::updateBotName);
	connect(bot, &Bot::statusChanged, this, &BotWidget::updateTabIconBasedOnBotStatus);
	connect(bot, &Bot::audioPlaybackRequested, this, &AbstractBotWidget::playWavSound);
	connect(bot, &Bot::audioStopRequested, this, &AbstractBotWidget::stopWavSound);

	connect(bot, &Bot::stopped, &runtimer, &QTimer::stop);
	connect(&runtimer, &QTimer::timeout, this, &BotWidget::runtimerTimedOut);
	connect(&runtimer, &QTimer::timeout, this, &BotWidget::tryBotStop);

	connect(m_bot_settings_widget, &BotSettingsWidget::settingsChanged, this, &BotWidget::updateShortcuts);
	connect(m_stop_hotkey, &QHotkey::activated, this, &BotWidget::stopHotkeyActivated);
	updateShortcuts(); // To initially setup the shortcuts

	updateBotName(bot->name());
}

void BotWidget::updateBotName(const QString &new_bot_name)
{
	QString formated_bot_name = QString("<span style='font-size:12pt;'>%0</span>").arg(new_bot_name);
	m_corner_widget->setText(formated_bot_name);
}

void BotWidget::updateTabIconBasedOnBotStatus(const Bot::Status &status)
{
	int updates_widget_index = m_tab_widget->indexOf(m_bot_updates_widget);
	QIcon tab_icon = status == Bot::Status::Outdated ? QIcon(":/icons/icons/icons8-exclamation-mark-32.png") : QIcon();
	m_tab_widget->setTabIcon(updates_widget_index, tab_icon);
}

void BotWidget::updateShortcuts()
{
	QKeySequence stop_ks = QKeySequence::fromString(m_bot_settings->value("stop_shortcut").toString());
	m_stop_hotkey->resetShortcut(); // required, otherwise the new shortcut will not override the old one
	m_stop_hotkey->setShortcut(stop_ks);
	m_stop_hotkey->setRegistered(!stop_ks.isEmpty());
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
		m_bot_log_widget->log(limitation_msg, Engine::LogSource::System);
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

void BotWidget::stopHotkeyActivated()
{
	if (!m_bot->isRunning())
	{
		qDebug() << "Can't stop not running bot via hotkey";
		return;
	}

	m_trayicon->showMessage(
		tr("Bot stopped via hotkey"),
		tr("%0 has been stopped via hotkey").arg(m_bot->name()),
		windowIcon()
	);

	tryBotStop();
}

void BotWidget::runtimerTimedOut()
{
	int runtime_in_minutes = qRound(runtimer.interval() / 1000 / 60.0);
	QString msg1 = QString("The bot stopped after %0 minutes, because you were not logged in.").arg(runtime_in_minutes);
	QString msg2 = QString("Go to Account > Login to prevent the bot from stopping after %0 minutes.").arg(runtime_in_minutes);
	m_bot_log_widget->log(msg1, Engine::LogSource::System);
	m_bot_log_widget->log(msg2, Engine::LogSource::System);
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

void BotWidget::setupConfigTab()
{
	int index = m_tab_widget->indexOf(m_bot_config_widget);

	if (index != -1) m_tab_widget->removeTab(index);
	if (m_bot_config_widget) m_bot_config_widget->deleteLater();

	m_bot_config_widget = new BotConfigWidget(m_bot, this);

	if (index == -1) m_tab_widget->addTab(m_bot_config_widget, "Config");
	else m_tab_widget->insertTab(index, m_bot_config_widget, "Config");

	index = m_tab_widget->indexOf(m_bot_config_widget);
	m_tab_widget->setTabEnabled(index, m_bot->manifest().config().isValid());
}
