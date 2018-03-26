#ifndef BFP__GUI__CONTROL_WINDOW_H
#define BFP__GUI__CONTROL_WINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "../scripting/bot.h"
#include "../scripting/bot_thread.h"

class BrowserWindow;
class AndroidDialog;

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
	void open(bool stable, bool trial);
	void on_actionStart_triggered();
	void bot_started();
	void on_actionStop_triggered();
	void on_actionKill_triggered();
	void bot_stopped(bool without_errors);
	void on_actionScripts_triggered();
	void on_actionSettings_triggered();
	void on_actionBrowser_triggered();
	void on_actionAndroid_triggered();
	void on_save_button_clicked();
	void on_clear_button_clicked();
	void appendMessage(QString log_message, bool from_client = false);
	void on_actionAbout_triggered();
	void on_actionAboutQt_triggered();
	
private:
	Ui::ControlWindow *ui;
	QFileDialog *file_dialog;
	BrowserWindow *browser_window;
	AndroidDialog *android_dialog;
	BotThread *bot_thread;
	bool trial = true;
};

#endif // BFP__GUI__CONTROL_WINDOW_H
