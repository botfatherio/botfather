#ifndef BFP__GUI__CONFIG_DIALOG_H
#define BFP__GUI__CONFIG_DIALOG_H

#include <QDialog>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit ConfigDialog(QWidget *parent = 0);
	~ConfigDialog();
	
public slots:
	void on_cancel_clicked();
	void on_okay_clicked();
	void on_adb_binary_browse_button_clicked();
	
private:
	Ui::ConfigDialog *ui;
	void saveConfig();
	void loadConfig();
};

#endif // BFP__GUI__CONFIG_DIALOG_H
