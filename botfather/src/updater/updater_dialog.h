#ifndef UPDATER_DIALOG_H
#define UPDATER_DIALOG_H

#include <QDialog>
#include "update_checker.h"

namespace Ui {
class UpdaterDialog;
}

class UpdaterDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit UpdaterDialog(QWidget *parent = 0);
	~UpdaterDialog();
	
public slots:
	void onUpdatesAvailable();
	void onUpdateCheckFailed(UpdateChecker::ErrorType);
	void disconnectUpdateChecker();
	
private:
	Ui::UpdaterDialog *ui;
	UpdateChecker *update_checker;
};

#endif // UPDATER_DIALOG_H
