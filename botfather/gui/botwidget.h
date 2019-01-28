#ifndef BOTWIDGET_H
#define BOTWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QMediaPlayer>
#include <QSettings>
#include "models/bot.h"
#include "../engine/engine.h"

namespace Ui {
class BotWidget;
}

class BotWidget : public QWidget
{
	Q_OBJECT

public:
	explicit BotWidget(Bot *bot, QWidget *parent = nullptr);
	~BotWidget();

public slots:
	void updateBotName(const QString &new_bot_name);
	void loadBotSettings();
	void saveBotSettings();
	void tryBotStart(int runtime_in_secs = -1);
	void tryBotStop();
	void runtimerTimedOut();
	void log(const QString &message, const Engine::LogSource &source);
	void saveLogToFile();
	void playWavSound(const QString &path_to_wav_file);
	void stopWavSound();
	void checkPermissions(const QString &script_path);

private:
	Ui::BotWidget *ui;
	Bot *m_bot;
	QMediaPlayer *media_player;
	QTimer runtimer;
	QSettings *bot_settings;
};

#endif // BOTWIDGET_H
