#ifndef BFP__GUI__CONTROL_WINDOW_H
#define BFP__GUI__CONTROL_WINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QSoundEffect>
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
	
signals:
	void loggedOut();
	
public slots:
	void applyRemoteApiInfo(int curtime, int premend, bool stable);
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
	void on_actionPremiumPlans_triggered();
	void on_actionLogout_triggered();
	void playWavSound(QString path_to_wav_file);
	void stopWavSound();
	void stopKillTimer();
	
private:
	Ui::ControlWindow *ui;
	QFileDialog *file_dialog;
	BrowserWindow *browser_window;
	AndroidDialog *android_dialog;
	Bot* bot;
	BotThread *bot_thread;
	bool trial = true;
	QString original_window_title;
	QSoundEffect *script_sound_effect;
	QTimer *kill_timer;
};

#endif // BFP__GUI__CONTROL_WINDOW_H
