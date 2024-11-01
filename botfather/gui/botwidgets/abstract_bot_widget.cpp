#include "abstract_bot_widget.hpp"

AbstractBotWidget::AbstractBotWidget(Bot *bot, QSystemTrayIcon *trayicon,
                                     QWidget *parent)
    : QWidget(parent), m_bot(bot), m_trayicon(trayicon) {
    m_media_player = new QMediaPlayer(this);
}

void AbstractBotWidget::playWavSound(const QString &path_to_wav_file) {
    // QMediaPlayer does only work in the main thread.
    stopWavSound();
    m_media_player->setMedia(QUrl::fromLocalFile(path_to_wav_file));
    m_media_player->play();
}

void AbstractBotWidget::stopWavSound() { m_media_player->stop(); }
