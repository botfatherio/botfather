#ifndef ABSTRACTBOTWIDGET_H
#define ABSTRACTBOTWIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include "../models/bot.h"

class AbstractBotWidget : public QWidget
{
	Q_OBJECT

public:
	AbstractBotWidget(Bot *bot, QWidget *parent = nullptr);
	virtual ~AbstractBotWidget() {}

public slots:
	virtual void playWavSound(const QString &path_to_wav_file);
	virtual void stopWavSound();

protected:
	Bot *m_bot;

private:
	QMediaPlayer *m_media_player;
};

#endif // ABSTRACTBOTWIDGET_H
