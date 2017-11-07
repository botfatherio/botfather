#ifndef BFP__GUI__CONTROL_WINDOW_H
#define BFP__GUI__CONTROL_WINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QThread>
#include <QFileDialog>
#include "config_dialog.h"
#include "scripting/bot.h"

namespace Ui {
class ControlWindow;
}

class ControlWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit ControlWindow(QWidget *parent = 0);
	~ControlWindow();
	
public slots:
	void on_actionHome_triggered();
	void on_actionReload_triggered();
	void on_actionStart_triggered();
	void bot_started();
	void on_actionStop_triggered();
	void bot_stopped();
	void on_actionSettings_triggered();
	void on_actionScripts_triggered();
	void on_actionSupport_triggered();
	void on_actionAbout_triggered();
	
private:
	QSettings settings;
	Ui::ControlWindow *ui;
	QFileDialog file_dialog;
	ConfigDialog config_dialog;
	QThread *bot_thread;
};

#endif // BFP__GUI__CONTROL_WINDOW_H
