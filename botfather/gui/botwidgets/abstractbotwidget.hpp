#ifndef ABSTRACTBOTWIDGET_H
#define ABSTRACTBOTWIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QSystemTrayIcon>
#include "../models/bot.hpp"

class AbstractBotWidget : public QWidget
{
	Q_OBJECT

public:
	AbstractBotWidget(Bot *bot, QSystemTrayIcon *trayicon, QWidget *parent = nullptr);
	virtual ~AbstractBotWidget() {}

public slots:
	virtual void playWavSound(const QString &path_to_wav_file);
	virtual void stopWavSound();

protected:
	Bot *m_bot;
	QSystemTrayIcon *m_trayicon;

private:
	QMediaPlayer *m_media_player;
};

#endif // ABSTRACTBOTWIDGET_H
