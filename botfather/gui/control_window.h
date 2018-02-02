#ifndef BFP__GUI__CONTROL_WINDOW_H
#define BFP__GUI__CONTROL_WINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "scripting/bot.h"
#include "scripting/bot_thread.h"

class LogDialog;
class ConfigDialog;

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
	void on_actionStart_triggered();
	void bot_started();
	void on_actionStop_triggered();
	void on_actionKill_triggered();
	void bot_stopped(bool without_errors);
	void on_actionLog_triggered();
	void on_actionSettings_triggered();
	void on_actionScripts_triggered();
	void on_actionSupport_triggered();
	void on_actionAbout_triggered();
	
private:
	Ui::ControlWindow *ui;
	LogDialog* log_dialog;
	ConfigDialog* config_dialog;
	BotThread *bot_thread;
};

#endif // BFP__GUI__CONTROL_WINDOW_H
