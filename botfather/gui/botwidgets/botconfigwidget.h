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

	void renderConfig(const BotConfig &bot_config, QVBoxLayout *vbox_layout);
	void renderGroup(const BotConfigGroup &group, QVBoxLayout *vbox_layout);
	void renderSetting(const BotConfigSetting &setting, QFormLayout *form_layout);

	void renderBooleanSetting(const BotConfigSetting &setting, QFormLayout *form_layout);
	void renderNumberSetting(const BotConfigSetting &setting, QFormLayout *form_layout);
	void renderIntegerSetting(const BotConfigSetting &setting, QFormLayout *form_layout);
	void renderStringSetting(const BotConfigSetting &setting, QFormLayout *form_layout);
	void renderPasswordSetting(const BotConfigSetting &setting, QFormLayout *form_layout);
	void renderChoiceSetting(const BotConfigSetting &setting, QFormLayout *form_layout);

private:
	Ui::BotConfigWidget *ui;
};

#endif // BOTCONFIGWIDGET_H
