#include "helper_api.h"
#include <QFileInfo>
#include <QThread>
#include <QApplication>
#include <QMessageBox>
#include <QInputDialog>
#include <QSound>
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

void HelperAPI::sleep(int seconds)
{
	QThread::sleep(seconds);
}

void HelperAPI::msleep(int milliseconds)
{
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

void HelperAPI::playWavSound(QString path_to_wav_file)
{
	if (!fileExists(path_to_wav_file)) {
		return;
	}
	QSound::play(m_bot_p->normalisePath(path_to_wav_file));
}