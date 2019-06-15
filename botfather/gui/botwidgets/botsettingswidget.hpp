#ifndef BOTSETTINGSWIDGET_H
#define BOTSETTINGSWIDGET_H

#include <QWidget>
#include <QSettings>
#include "../models/bot.hpp"

namespace Ui {
class BotSettingsWidget;
}

class BotSettingsWidget : public QWidget
{
	Q_OBJECT

public:
	explicit BotSettingsWidget(Bot *bot, QWidget *parent = nullptr);
	~BotSettingsWidget();

public slots:
	void loadBotSettings();
	void saveBotSettings();

signals:
	void settingsChanged();

private:
	Ui::BotSettingsWidget *m_ui;
	QSettings *m_bot_settings;
};

#endif // BOTSETTINGSWIDGET_H
