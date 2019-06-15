#ifndef BF_GUI_PREFERENCES_DIALOG_H
#define BF_GUI_PREFERENCES_DIALOG_H

#include <QDialog>

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
	Q_OBJECT
	
public:
		explicit PreferencesDialog(QWidget *parent = nullptr);
		~PreferencesDialog();

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
	Ui::PreferencesDialog *ui;
};

#endif // BF_GUI_PREFERENCES_DIALOG_H
