#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QHash>
#include "models/botlistmodel.h"
#include "scriptmanager/scriptmanagerdialog.h"
#include "androiddialog/android_dialog.h"
#include "preferences/preferences_dialog.h"
#include "authdialog/auth_dialog.h"
#include "botwidgets/botwidget.h"
#include "maintenancetool/maintenancetool.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow() override;

public slots:
	void handleRowChange(const QModelIndex &current, const QModelIndex &previous);
	void deleteSelectedBot();
	void startSelectedBot();
	void stopSelectedBot();

	void syncButtonsWithBotState();
	void setupWidgetForBot(Bot *bot);
	void focusWidgetForBot(const QString &bot_path);
	void removeWidgetForBot(const QString &bot_path);

	void addLocalBot();
	void updateLicenseInfo();
	void showAboutDialog();
	void notifyAboutUpdate();

protected:
	void closeEvent(QCloseEvent *event) override;

private:
	Ui::MainWindow *ui;
	QSystemTrayIcon *m_tray_icon;
	BotListModel *m_bot_list_model;
	QHash<QString, BotWidget*> m_bot_path_to_widget_map;
	ScriptManagerDialog *m_script_manager_dialog;
	AndroidDialog *m_android_dialog;
	PreferencesDialog *m_preferences_dialog;
	LicenseApiClient *m_license_api_client;
	AuthDialog *m_auth_dialog;
	MaintenanceTool *m_maintenance_tool;
};

#endif // MAINWINDOW_H
