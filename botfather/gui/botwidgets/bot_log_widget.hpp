#ifndef BOTLOGWIDGET_H
#define BOTLOGWIDGET_H

#include <QWidget>
#include <QSettings>
#include "../models/bot.hpp"
#include "../../engine/engine.hpp"

namespace Ui {
class BotLogWidget;
}

class BotLogWidget : public QWidget
{
	Q_OBJECT

public:
	explicit BotLogWidget(Bot *bot, QWidget *parent = nullptr);
	~BotLogWidget();

public slots:
	void log(const QString &message, const Engine::LogSource &source);
	void saveLogToFile();

private:
	Ui::BotLogWidget *m_ui;
	QSettings *m_bot_settings;
};

#endif // BOTLOGWIDGET_H
