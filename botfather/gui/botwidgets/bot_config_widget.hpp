#ifndef BOTCONFIGWIDGET_H
#define BOTCONFIGWIDGET_H

#include <QWidget>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include "../models/bot.hpp"

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
	void renderOption(BotConfigOption *option_p, QFormLayout *form_layout);

	void renderBooleanOption(BotConfigOption *option_p, QFormLayout *form_layout);
	void renderNumberOption(BotConfigOption *option_p, QFormLayout *form_layout);
	void renderIntegerOption(BotConfigOption *option_p, QFormLayout *form_layout);
	void renderStringOption(BotConfigOption *option_p, QFormLayout *form_layout);
	void renderPasswordOption(BotConfigOption *option_p, QFormLayout *form_layout);
	void renderChoiceOption(BotConfigOption *option_p, QFormLayout *form_layout);

private:
	Ui::BotConfigWidget *ui;
	QVector<BotConfigOption*> m_bot_config_settings;
};

#endif // BOTCONFIGWIDGET_H
