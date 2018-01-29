#include "bot.h"
#include <QJSEngine>
#include <QFile>
#include <QFileInfo>
#include <QThread>
#include <QDebug>
#include "../shared/constants.h"
#include "../vision/vision_api.h"
#include "../browser/browser_api.h"

Bot::Bot(QString script_path) : m_script_path(script_path)
{}

bool Bot::fileExists(QString file_path)
{
	QFileInfo file(file_path);
	return file.exists() && file.isFile();
}

void Bot::sleep(int seconds)
{
	QThread::sleep(seconds);
}

void Bot::msleep(int milliseconds)
{
	QThread::msleep(milliseconds);
}

int Bot::getVersionMajor()
{
	return constants::VERSION_MAJOR;
}

int Bot::getVersionMinor()
{
	return constants::VERSION_MINOR;
}

int Bot::getVersionPatch()
{
	return constants::VERSION_PATCH;
}

QString Bot::getClientType()
{
	return constants::CLIENT_TYPE;
}

QString Bot::getClientMode()
{
	return "gui"; // TODO: Make this actually based on the clients mode.
}

void Bot::log(QString log_message)
{
	emit this->message(log_message);
}

void Bot::runScript()
{
	emit this->started();
	
	QJSEngine engine;
	engine.installExtensions(QJSEngine::ConsoleExtension);
	
	VisionAPI::enable(&engine);
	BrowserAPI::enable(&engine);
	
	QFile script_file(this->m_script_path);
	if (!script_file.open(QIODevice::ReadOnly)) {
		QString debug_msg("Can't read the contents from " + this->m_script_path + ". Please check the files permissions.");
		emit this->message(debug_msg);
		emit this->stopped(false);
		return;
	}
	
	QTextStream stream(&script_file);
	QString contents = stream.readAll();
	script_file.close();
	emit this->message("Executing bot script " + this->m_script_path);
	
	QJSValue result = engine.evaluate(contents, this->m_script_path);
	
	// Reset settings made by the script
	BrowserAPI::unmodifyRessources();
	
	if (result.isError()) {
		QString debug_msg("<b style='color:red'>Uncaught exception</b> at line " + result.property("lineNumber").toString() + ":" + result.toString());
		emit this->message(debug_msg);
		emit this->stopped(false);
		return;
	}
	
	emit this->message("Bot script execution finished.");
	emit this->stopped(true);
}