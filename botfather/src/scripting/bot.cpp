#include "bot.h"
#include <QtScript/QScriptEngine>
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include "../scripting/helper_api.h"
#include "../vision/vision_api.h"
#include "../browser/browser_api.h"
#include "../browser/browser.h"
#include "../android/android_api.h"

Bot::Bot(BotThread *thread_p, QString script_path) : m_thread_p(thread_p), m_script_path(script_path)
{}

void Bot::runScript()
{
	running = true;
	emit this->started();
	
	// Reset settings eventually made by a previously run script.
	Browser::unmodifyRessources();
	
	// Create script engine and enable our api in it's global context.
	QScriptEngine *script_engine = new QScriptEngine(this);
	HelperAPI::enable(this, m_thread_p, script_engine);
	VisionAPI::enable(this, script_engine);
	BrowserAPI::enable(this, script_engine);
	AndroidAPI::enable(this, script_engine);
	
	// Register custom QObject based types.
	qScriptRegisterMetaType(script_engine, Image::toScriptValue, Image::fromScriptValue);
	
	// Try to open the submitted script file.
	QFile script_file(this->m_script_path);
	if (!script_file.open(QIODevice::ReadOnly)) {
		QString debug_msg("Can't read the contents from " + this->m_script_path + ". Please check the files permissions.");
		emit this->message(debug_msg, true);
		emit this->stopped(false);
		return;
	}
	
	// Get the script files contents.
	QTextStream stream(&script_file);
	QString contents = stream.readAll();
	script_file.close();
	emit this->message("Executing bot script " + this->m_script_path, true);
	
	// Enable termination for the thread the bot and thus the script is running in,
	// so that the user can kill the script when it's stuck or doesn't react to
	// script interruption requests.
	m_thread_p->setTerminationEnabled(true);
	
	// Run the script.
	QScriptValue result;
	try{
		result = script_engine->evaluate(contents, this->m_script_path);
	}
	catch (...) {
		QString debug_msg(
			"<b style='color:red'>Internal exception cought!</b> You script nearly crashed botfather."
			"<b style='color:green'>Please send us your script, so that the problem can be solved."
		);
		emit this->message(debug_msg, true);
		emit this->stopped(false);
		return;
	}
	
	// Check whether the script ended due to errors. If so print them to the users log.
	if (result.isError()) {
		QString debug_msg("<b style='color:red'>Uncaught exception</b> at line " + result.property("lineNumber").toString() + ": " + result.toString());
		emit this->message(debug_msg, true);
		emit this->stopped(false);
		return;
	}

	// Script execution/evaluation ended successfully.
	running = false;
	emit this->message("Bot script execution finished.", true);
	emit this->stopped(true);
}

bool Bot::isRunning() const {
	return running;
}

QString Bot::normalisePath(QString path)
{
	QFileInfo file_info(path);
	if (file_info.isRelative()) {
		return QDir(this->getAbsoluteScriptDirPath()).filePath(path);
	}
	return path;
}

QString Bot::getAbsoluteScriptDirPath()
{
	QFileInfo file_info(this->m_script_path);
	return file_info.dir().absolutePath();
}

bool Bot::fileExists(QString file_path)
{
	file_path = this->normalisePath(file_path);
	QFileInfo file_info(file_path);
	return file_info.exists() && file_info.isFile();
}