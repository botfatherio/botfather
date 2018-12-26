#ifndef BFP_GUI_SETTINGSDIALOG_SETTINGS_DIALOG_H
#define BFP_GUI_SETTINGSDIALOG_SETTINGS_DIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
	Q_OBJECT
	
public:
        explicit SettingsDialog(QWidget *parent = nullptr);
        ~SettingsDialog();

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
        Ui::SettingsDialog *ui;
};

#endif // BFP_GUI_SETTINGSDIALOG_SETTINGS_DIALOG_H
