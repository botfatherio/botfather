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
	explicit ConfigDialog(QWidget *parent = nullptr);
	~ConfigDialog();

signals:
	void configLoaded();
	void configSaved();
	
public slots:
	void on_browse_adb_binary_pressed();
	void on_browse_flash_so_pressed();
	void on_browse_flash_manifest_pressed();
	void saveConfig();
	void loadConfig();
	
private:
	Ui::ConfigDialog *ui;
};

#endif // BFP__GUI__CONFIG_DIALOG_H
