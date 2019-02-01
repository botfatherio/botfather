#ifndef BOTCONFIGWIDGET_H
#define BOTCONFIGWIDGET_H

#include <QWidget>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include "../models/bot.h"

namespace Ui {
class BotConfigWidget;
}

class BotConfigWidget : public QWidget
{
	Q_OBJECT

public:
	explicit BotConfigWidget(Bot *bot, QWidget *parent = nullptr);
	~BotConfigWidget();

	void renderConfig(BotConfig bot_config, QVBoxLayout *vbox_layout);
	void renderGroup(BotConfigGroup group, QVBoxLayout *vbox_layout);
	void renderSetting(BotConfigSetting *setting, QFormLayout *form_layout);

	void renderBooleanSetting(BotConfigSetting *setting, QFormLayout *form_layout);
	void renderNumberSetting(BotConfigSetting *setting, QFormLayout *form_layout);
	void renderIntegerSetting(BotConfigSetting *setting, QFormLayout *form_layout);
	void renderStringSetting(BotConfigSetting *setting, QFormLayout *form_layout);
	void renderPasswordSetting(BotConfigSetting *setting, QFormLayout *form_layout);
	void renderChoiceSetting(BotConfigSetting *setting, QFormLayout *form_layout);

private:
	Ui::BotConfigWidget *ui;
	QVector<BotConfigSetting*> m_bot_config_settings;
};

#endif // BOTCONFIGWIDGET_H
