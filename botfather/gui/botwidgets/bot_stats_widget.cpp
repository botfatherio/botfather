#include "bot_stats_widget.hpp"
#include "ui_bot_stats_widget.h"
#include <QVBoxLayout>

BotStatsWidget::BotStatsWidget(Bot* bot, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BotStatsWidget)
{
    ui->setupUi(this);
    connect(bot, &Bot::statsUpdated, this, &BotStatsWidget::statsUpdated);
}

BotStatsWidget::~BotStatsWidget()
{
    delete ui;
}

void BotStatsWidget::statsUpdated(const QString &group, const QString &label, const QString &value) {
    QLabel* label_widget = getOrCreateLabelWidget(group, label);
    QString label_text = QString("%1: %2").arg(label, value);
    label_widget->setText(label_text);
}

QGroupBox* BotStatsWidget::getOrCreateGroupWidget(const QString &group) {
    if (!group_map.contains(group)) {
        QGroupBox* group_widget = new QGroupBox(group, ui->stats_container);
        QVBoxLayout *group_box_layout = new QVBoxLayout(group_widget);
        group_widget->setLayout(group_box_layout);
        ui->stats_container->layout()->addWidget(group_widget);
        group_map[group] = group_widget;
    }
    return group_map[group];
}

QLabel* BotStatsWidget::getOrCreateLabelWidget(const QString &group, const QString &label) {
    QGroupBox *group_widget = getOrCreateGroupWidget(group);
    if (!label_map.contains(group)) {
        label_map[group] = QHash<QString, QLabel*>();
    }
    if (!label_map[group].contains(label)) {
        QLabel* label_widget = new QLabel(group_widget);
        group_widget->layout()->addWidget(label_widget);
        label_map[group][label] = label_widget;
    }
    return label_map[group][label];
}
