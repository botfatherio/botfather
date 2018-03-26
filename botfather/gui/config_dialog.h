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
	void on_adb_binary_browse_button_clicked();
	void saveConfig();
	void loadConfig();
	
private:
	Ui::ConfigDialog *ui;
};

#endif // BFP__GUI__CONFIG_DIALOG_H
