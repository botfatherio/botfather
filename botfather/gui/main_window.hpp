#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHash>
#include <QMainWindow>
#include <QSystemTrayIcon>

#include "androiddialog/android_dialog.hpp"
#include "botwidgets/bot_widget.hpp"
#include "maintenancetool/maintenance_tool.hpp"
#include "models/bot_list_model.hpp"
#include "preferences/preferences_dialog.hpp"
#include "scriptmanager/script_manager_dialog.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

   public slots:
    void handleRowChange(const QModelIndex &current,
                         const QModelIndex &previous);
    void deleteSelectedBot();
    void startSelectedBot();
    void stopSelectedBot();

    void syncButtonsWithBotState();
    void setupWidgetForBot(Bot *bot);
    void focusWidgetForBot(const QString &bot_path);
    void removeWidgetForBot(const QString &bot_path);

    void addLocalBot();
    void openDonateLink();
    void showAboutDialog();
    void openColorFinder();
    void notifyAboutUpdate();

   protected:
    void closeEvent(QCloseEvent *event) override;

   private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *m_tray_icon;
    BotListModel *m_bot_list_model;
    QHash<QString, BotWidget *> m_bot_path_to_widget_map;
    ScriptManagerDialog *m_script_manager_dialog;
    AndroidDialog *m_android_dialog;
    PreferencesDialog *m_preferences_dialog;
    MaintenanceTool *m_maintenance_tool;
};

#endif  // MAINWINDOW_H
