#include "helper_api.h"
#include <QFileInfo>
#include <QThread>
#include "bot.h"
#include "bot_thread.h"
#include "../shared/constants.h"

HelperAPI::HelperAPI(BotThread* bot_thread_p, Bot* bot_p, QJSEngine* engine_p) : m_bot_thread_p(bot_thread_p), m_bot_p(bot_p), m_engine_p(engine_p)
{}

// static
void HelperAPI::enable(BotThread* bot_thread_p, Bot* bot_p, QJSEngine* engine_p)
{
	QJSValue vision_obj = engine_p->newQObject(new HelperAPI(bot_thread_p, bot_p, engine_p));
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

int HelperAPI::getVersionMajor()
{
	return constants::VERSION_MAJOR;
}

int HelperAPI::getVersionMinor()
{
	return constants::VERSION_MINOR;
}

int HelperAPI::getVersionPatch()
{
	return constants::VERSION_PATCH;
}

QString HelperAPI::getClientType()
{
	return constants::CLIENT_TYPE;
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