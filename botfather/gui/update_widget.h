#ifndef BFP__GUI__UPDATE_WIDGET_H
#define BFP__GUI__UPDATE_WIDGET_H

#include <QWidget>
#include "../updater/update_checker.h"

namespace Ui {
class UpdateWidget;
}

class UpdateWidget : public QWidget
{
	Q_OBJECT
	
public:
	explicit UpdateWidget(QWidget *parent = 0);
	~UpdateWidget();
	
signals:
	void finished();
	
public slots:
	void checkForUpdates();
	void informAboutUpdate();
	void informAboutError(UpdateChecker::ErrorType error_type);
	void runUpdater();
	
private:
	Ui::UpdateWidget *ui;
	UpdateChecker *update_checker;
};

#endif // BFP__GUI__UPDATE_WIDGET_H
