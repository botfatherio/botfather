#include "helper_api.h"
#include <QFileInfo>
#include <QThread>
#include <QApplication>
#include <QMessageBox>
#include <QInputDialog>
#include <QAudioBuffer>
#include <QDebug>
#include "bot.h"
#include "bot_thread.h"

HelperAPI::HelperAPI(Bot* bot_p, BotThread* bot_thread_p, QScriptEngine* engine_p)
	: QObject(bot_p), m_bot_thread_p(bot_thread_p), m_bot_p(bot_p), m_engine_p(engine_p)
{}

// static
void HelperAPI::enable(Bot* bot_p, BotThread* bot_thread_p, QScriptEngine* engine_p)
{
	QScriptValue vision_obj = engine_p->newQObject(new HelperAPI(bot_p, bot_thread_p, engine_p));
	engine_p->globalObject().setProperty("Helper", vision_obj);
}

bool HelperAPI::fileExists(QString file_path)
{
	file_path = this->m_bot_p->normalisePath(file_path);
	return this->m_bot_p->fileExists(file_path);
}

void HelperAPI::sleep(int seconds, bool deep)
{
	if (seconds <= 0) {
		m_engine_p->currentContext()->throwError("Timeout must be at least 1 second.");
		return;
	}
	if (deep) {
		QThread::sleep(seconds);
		return;
	}
	
	// Sleep in 250ms intervals while checking whether stop has been requested.
	// We do so to make stopping a script while it's sleeping look more fluent.
	for (int i = 0; i < seconds * 4; i++) {
		if (stopRequested()) {
			break;
		}
		QThread::msleep(250);
	}
}

void HelperAPI::msleep(int milliseconds)
{
	if (milliseconds <= 0) {
		m_engine_p->currentContext()->throwError("Timeout must be at least 1 millisecond.");
		return;
	}
	QThread::msleep(milliseconds);
}

QString HelperAPI::getVersion()
{
	return QApplication::applicationVersion();
}

QString HelperAPI::getClientMode()
{
	return "gui"; // TODO: Make this actually based on the clients mode.
}

void HelperAPI::log(QString log_message)
{
	emit m_bot_p->message(log_message, false);
}

bool HelperAPI::stopRequested()
{
	return m_bot_thread_p->isInterruptionRequested();
}

QString HelperAPI::getAbsoluteScriptDirPath()
{
	return m_bot_p->getAbsoluteScriptDirPath();
}

void HelperAPI::playWavSound(QString path_to_wav_file, bool blocking)
{
	if (!fileExists(path_to_wav_file)) {
		m_engine_p->currentContext()->throwError("Wav file does not exist.");
		return;
	}
	
	emit m_bot_p->playWavSound(m_bot_p->normalisePath(path_to_wav_file));
	
	if (blocking) {
		QFile wav_file(m_bot_p->normalisePath(path_to_wav_file));
		if(!wav_file.open(QIODevice::ReadOnly)) {
			// Reading the wav file failed.
			return;
		}
		
		// Common wav file header (checked this using VLC player).
		QAudioFormat format;
		format.setSampleSize(16);
		format.setSampleRate(44100);
		format.setChannelCount(2);
		format.setCodec("audio/pcm");
		format.setByteOrder(QAudioFormat::LittleEndian);
		format.setSampleType(QAudioFormat::UnSignedInt);
		
		QAudioBuffer buffer(wav_file.readAll(), format);
		//qDebug() << "DURATION:" << buffer.duration() / 1000 / 1000 << "seconds";
		//QThread::usleep(buffer.duration());
		sleep(buffer.duration() / 1000 / 1000, false);
	}
}

void HelperAPI::stopWavSound()
{
	emit m_bot_p->stopWavSound();
}