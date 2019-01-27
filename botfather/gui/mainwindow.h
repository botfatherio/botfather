#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHash>
#include "models/botlistmodel.h"
#include "scriptmanager/scriptmanagerdialog.h"
#include "browserwindow/browser_window.h"
#include "androiddialog/android_dialog.h"
#include "settingsdialog/settings_dialog.h"
#include "authdialog/auth_dialog.h"
#include "../tools/mtoolwrapper.h"
#include "botwidget.h"

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

	void setupWidgetForBot(Bot *bot);
	void focusWidgetForBot(const QString &bot_path);
	void removeWidgetForBot(const QString &bot_path);

	void addLocalBot();
	void adjustLimitations(bool is_premium = false);
	void showAboutDialog();

private:
	Ui::MainWindow *ui;
	BotListModel *m_bot_list_model;
	QHash<QString, BotWidget*> m_bot_path_to_widget_map;
	ScriptManagerDialog *m_script_manager_dialog;
	BrowserWindow *m_browser_window;
	AndroidDialog *m_android_dialog;
	SettingsDialog *m_settings_dialog;
	AuthDialog* m_auth_dialog;
	MtoolWrapper* m_maintainance_tool;
};

#endif // MAINWINDOW_H
