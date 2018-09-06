#ifndef BFP__GUI__CONTROL_WINDOW_H
#define BFP__GUI__CONTROL_WINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QSoundEffect>
#include <QHotkey>

class Bot;
class ConfigDialog;
class BrowserWindow;
class AndroidDialog;

namespace Ui {
	class ControlWindow;
}

class ControlWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit ControlWindow(QWidget *parent = nullptr);
	~ControlWindow();
	
signals:
	void loggedOut();
	
public slots:
	void applyRemoteApiInfo(int curtime, int premend, bool stable);
	void on_actionStart_triggered();
	void bot_started();
	void on_actionStop_triggered();
	void bot_stopped(bool without_errors);
	void on_actionScripts_triggered();
	void on_save_button_clicked();
	void appendMessage(QString message, bool from_botfather, bool error = false);
	void on_actionAbout_triggered();
	void on_actionAboutQt_triggered();
	void on_actionPremiumPlans_triggered();
	void on_actionLogout_triggered();
	void playWavSound(QString path_to_wav_file);
	void stopWavSound();
	void updateHotkeys();
	
private:
	Ui::ControlWindow *ui;
	ConfigDialog *config_dialog;
	BrowserWindow *browser_window;
	AndroidDialog *android_dialog;
	Bot* bot = nullptr;
	QThread *bot_thread = nullptr;
	bool trial = true;
	QSoundEffect *script_sound_effect;
	QHotkey *stop_hotkey;
};

#endif // BFP__GUI__CONTROL_WINDOW_H
