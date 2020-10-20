#ifndef BOTUPDATESWIDGET_H
#define BOTUPDATESWIDGET_H

#include <QSettings>
#include <QWidget>

#include "../models/bot.hpp"

namespace Ui {
class BotUpdatesWidget;
}

class BotUpdatesWidget : public QWidget {
    Q_OBJECT

   public:
    explicit BotUpdatesWidget(Bot *bot, QWidget *parent = nullptr);
    ~BotUpdatesWidget();

   public slots:
    void checkForUpdates();
    void updateTheBot();
    void resetProgressIndicators();
    void botStatusChanged(const Bot::Status &status);

   signals:
    void botUpdated();

   private:
    Ui::BotUpdatesWidget *ui;
    QSettings *m_bot_settings;
    Bot *m_bot;
};

#endif  // BOTUPDATESWIDGET_H
