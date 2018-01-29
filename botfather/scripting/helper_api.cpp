#include "helper_api.h"
#include <QFileInfo>
#include <QThread>
#include "../shared/constants.h"

HelperAPI::HelperAPI(QJSEngine* engine_p) : m_engine_p(engine_p)
{}

// static
void HelperAPI::enable(QJSEngine* engine_p)
{
	QJSValue vision_obj = engine_p->newQObject(new HelperAPI(engine_p));
	engine_p->globalObject().setProperty("Helper", vision_obj);
}

bool HelperAPI::fileExists(QString file_path)
{
	QFileInfo file(file_path);
	return file.exists() && file.isFile();
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
	// TODO: Implement this
}