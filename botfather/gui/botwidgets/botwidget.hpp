#ifndef BOTWIDGET_H
#define BOTWIDGET_H

#include <QTabWidget>
#include <QLabel>
#include <QTimer>
#include <QSettings>
#include <QHotkey>
#include "abstractbotwidget.hpp"
#include "botlogwidget.hpp"
#include "botconfigwidget.hpp"
#include "botupdateswidget.hpp"
#include "botbrowserswidget.hpp"
#include "botsettingswidget.hpp"
#include "../../engine/engine.hpp"

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

protected:
	void setupConfigTab();

private:
	QTabWidget *m_tab_widget;
	QLabel *m_corner_widget;
	BotLogWidget *m_bot_log_widget;
	BotConfigWidget *m_bot_config_widget = nullptr;
	BotUpdatesWidget *m_bot_updates_widget;
	BotBrowsersWidget *m_bot_browsers_widget;
	BotSettingsWidget *m_bot_settings_widget;
	QTimer runtimer;
	QSettings *m_bot_settings;
	QHotkey *m_stop_hotkey;
};

#endif // BOTWIDGET_H