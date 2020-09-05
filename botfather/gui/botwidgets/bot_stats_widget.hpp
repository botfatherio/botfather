#ifndef BOT_STATS_WIDGET_H
#define BOT_STATS_WIDGET_H

#include <QWidget>
#include <QHash>
#include <QGroupBox>
#include <QLabel>
#include "../models/bot.hpp"

namespace Ui {
class BotStatsWidget;
}

class BotStatsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BotStatsWidget(Bot* bot, QWidget *parent = nullptr);
    ~BotStatsWidget();

public slots:
    void clearStats();
    void statsUpdated(const QString &group, const QString &label, const QString &value);

protected:
    QGroupBox* getOrCreateGroupWidget(const QString &group);
    QLabel* getOrCreateLabelWidget(const QString &group, const QString &label);

private:
    Ui::BotStatsWidget *ui;
    QHash<QString, QGroupBox*> group_map;
    QHash<QString, QHash<QString, QLabel*>> label_map;
};

#endif // BOT_STATS_WIDGET_H
