#ifndef BOTWIDGET_H
#define BOTWIDGET_H

#include <QTabWidget>
#include <QLabel>
#include <QTimer>
#include <QSettings>
#include <QHotkey>
#include "abstractbotwidget.h"
#include "botlogwidget.h"
#include "botconfigwidget.h"
#include "botupdateswidget.h"
#include "botsettingswidget.h"
#include "../../engine/engine.h"

class BotWidget : public AbstractBotWidget
{
	Q_OBJECT

public:
	explicit BotWidget(Bot *bot, QSystemTrayIcon *trayicon, QWidget *parent = nullptr);

public slots:
	void updateBotName(const QString &new_bot_name);
	void updateTabIconBasedOnBotStatus(const Bot::Status &status);
	void updateShortcuts();
	void tryBotStart(int runtime_in_secs = -1);
	void tryBotStop();
	void stopHotkeyActivated();
	void runtimerTimedOut();
	void checkPermissions(const QString &script_path);

private:
	QTabWidget *m_tab_widget;
	QLabel *m_corner_widget;
	BotLogWidget *m_bot_log_widget;
	BotConfigWidget *m_bot_config_widget;
	BotUpdatesWidget *m_bot_updates_widget;
	BotSettingsWidget *m_bot_settings_widget;
	QTimer runtimer;
	QSettings *m_bot_settings;
	QHotkey *m_stop_hotkey;
};

#endif // BOTWIDGET_H
