#ifndef BOTBROWSERSWIDGET_H
#define BOTBROWSERSWIDGET_H

#include <QWidget>
#include "../models/bot.h"

namespace Ui {
class BotBrowsersWidget;
}

class BotBrowsersWidget : public QWidget
{
	Q_OBJECT

public:
	explicit BotBrowsersWidget(Bot *bot, QWidget *parent = nullptr);
	~BotBrowsersWidget();

public slots:
	void viewBrowser(const QModelIndex &index);

private:
	Ui::BotBrowsersWidget *ui;
};

#endif // BOTBROWSERSWIDGET_H
