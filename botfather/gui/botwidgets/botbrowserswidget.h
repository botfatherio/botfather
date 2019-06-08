#ifndef BOTBROWSERSWIDGET_H
#define BOTBROWSERSWIDGET_H

#include <QWidget>

namespace Ui {
class BotBrowsersWidget;
}

class BotBrowsersWidget : public QWidget
{
	Q_OBJECT

public:
	explicit BotBrowsersWidget(QWidget *parent = nullptr);
	~BotBrowsersWidget();

public slots:
	void viewBrowser(const QModelIndex &index);

private:
	Ui::BotBrowsersWidget *ui;
};

#endif // BOTBROWSERSWIDGET_H
