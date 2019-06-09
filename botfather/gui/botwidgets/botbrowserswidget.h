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
	void handleRowChange(const QModelIndex &current, const QModelIndex &previous);
	void deleteCurrentBrowser();
	void deleteBrowser(const QModelIndex &index);
	void viewCurrentBrowser();
	void viewBrowser(const QModelIndex &index);

private:
	Ui::BotBrowsersWidget *ui;
};

#endif // BOTBROWSERSWIDGET_H
