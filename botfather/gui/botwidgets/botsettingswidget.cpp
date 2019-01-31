#include "botsettingswidget.h"
#include "ui_botsettingswidget.h"

BotSettingsWidget::BotSettingsWidget(Bot *bot, QWidget *parent)
	: QWidget(parent)
	, m_ui(new Ui::BotSettingsWidget)
{
	m_ui->setupUi(this);
	m_bot_settings = new QSettings(bot->settingsPath(), QSettings::IniFormat);

	connect(m_ui->clear_stop_shortcut, &QPushButton::clicked, m_ui->stop_shortcut, &QKeySequenceEdit::clear);

	// Storing the bot setting when the application is about to quit delays the quit noticeable
	//connect(ui->debug_mode, &QCheckBox::clicked, this, &BotWidget::saveBotSettings);
	connect(m_ui->auto_update_check, &QCheckBox::clicked, this, &BotSettingsWidget::saveBotSettings);
	connect(m_ui->clear_stop_shortcut, &QPushButton::clicked, this, &BotSettingsWidget::saveBotSettings);
	connect(m_ui->stop_shortcut, &QKeySequenceEdit::editingFinished, this, &BotSettingsWidget::saveBotSettings);

	loadBotSettings();
}

BotSettingsWidget::~BotSettingsWidget()
{
	delete m_ui;
}

void BotSettingsWidget::loadBotSettings()
{
	//ui->debug_mode->setChecked(bot_settings->value("debug_mode", false).toBool());
	m_ui->auto_update_check->setChecked(m_bot_settings->value("auto_update_check", true).toBool());
	m_ui->stop_shortcut->setKeySequence(QKeySequence::fromString(m_bot_settings->value("stop_shortcut").toString()));
}

void BotSettingsWidget::saveBotSettings()
{
	//bot_settings->setValue("debug_mode", ui->debug_mode->isChecked());
	m_bot_settings->setValue("auto_update_check", m_ui->auto_update_check->isChecked());
	m_bot_settings->setValue("stop_shortcut", m_ui->stop_shortcut->keySequence().toString());
	emit settingsChanged();
}
