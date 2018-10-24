#ifndef BFP__GUI__CONTROL_WINDOW_H
#define BFP__GUI__CONTROL_WINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QHotkey>
#include <QTimer>

#include "../engine/bot.h"

class BrowserWindow;

namespace Ui {
	class ControlWindow;
}

class ControlWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit ControlWindow(QWidget *parent = nullptr);
	~ControlWindow();
	
public slots:
	void adjustLimitations(bool is_premium);
	void onLoggedIn(int curtime, int premend);
	void onLogout();
	void startBot();
	void botStarted();
	void stopBot();
	void botStopped(bool without_errors);
	void appendMessage(const QString &message, const Bot::LogSource &source);
	void on_save_button_clicked();
	void on_actionScripts_triggered();
	void on_actionAbout_triggered();
	void on_actionAboutQt_triggered();
	void playWavSound(QString path_to_wav_file);
	void stopWavSound();
	void updateHotkeys();

protected:
    void closeEvent(QCloseEvent* event);
	
private:
	Ui::ControlWindow *ui;
	BrowserWindow *browser_window;
	Bot* bot = nullptr;
	QThread *bot_thread = nullptr;
	QMediaPlayer *media_player;
	QHotkey *stop_hotkey;
	QTimer runtimer;
};

#endif // BFP__GUI__CONTROL_WINDOW_H
