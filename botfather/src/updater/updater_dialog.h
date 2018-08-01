#ifndef BFP__UPDATER__UPDATER_DIALOG_H
#define BFP__UPDATER__UPDATER_DIALOG_H

#include <QDialog>
#include "update_checker.h"

namespace Ui {
class UpdaterDialog;
}

class UpdaterDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit UpdaterDialog(QWidget *parent = nullptr);
	~UpdaterDialog();
	
public slots:
	void onUpdatesAvailable();
	void onUpdateCheckFailed(UpdateChecker::ErrorType);
	void disconnectUpdateChecker();
	
private:
	Ui::UpdaterDialog *ui;
	UpdateChecker *update_checker;
};

#endif // BFP__UPDATER__UPDATER_DIALOG_H
