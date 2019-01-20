#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "models/botlistmodel.h"
#include "scriptmanager/scriptmanagerdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow() override;

private:
	Ui::MainWindow *ui;
	BotListModel *m_bot_list_model;

	ScriptManagerDialog *m_script_manager_dialog;
};

#endif // MAINWINDOW_H
