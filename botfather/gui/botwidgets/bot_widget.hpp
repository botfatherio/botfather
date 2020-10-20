#ifndef BOTWIDGET_H
#define BOTWIDGET_H

#include <QHotkey>
#include <QLabel>
#include <QSettings>
#include <QTabWidget>
#include <QTimer>

#include "../../engine/engine.hpp"
#include "abstract_bot_widget.hpp"
#include "bot_browsers_widget.hpp"
#include "bot_config_widget.hpp"
#include "bot_log_widget.hpp"
#include "bot_settings_widget.hpp"
#include "bot_stats_widget.hpp"
#include "bot_updates_widget.hpp"

class BotWidget : public AbstractBotWidget {
    Q_OBJECT

   public:
    explicit BotWidget(Bot *bot, QSystemTrayIcon *trayicon,
                       QWidget *parent = nullptr);

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
    BotStatsWidget *m_bot_stats_widget;
    BotBrowsersWidget *m_bot_browsers_widget;
    BotSettingsWidget *m_bot_settings_widget;
    QTimer runtimer;
    QSettings *m_bot_settings;
    QHotkey *m_stop_hotkey;
};

#endif  // BOTWIDGET_H
