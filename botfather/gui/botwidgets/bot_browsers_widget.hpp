#ifndef BOTBROWSERSWIDGET_H
#define BOTBROWSERSWIDGET_H

#include <QWidget>
#include <QSortFilterProxyModel>
#include "../models/bot.hpp"

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
	void updateButtonStates();
	void deleteCurrentBrowser();
	void deleteBrowser(const QModelIndex &index);
	void viewCurrentBrowser();
	void viewBrowser(const QModelIndex &index);

private:
	Ui::BotBrowsersWidget *ui;
	Bot *m_bot = nullptr;
	QSortFilterProxyModel *m_browser_model_proxy;
};

#endif // BOTBROWSERSWIDGET_H
