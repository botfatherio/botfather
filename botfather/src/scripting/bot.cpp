#include "bot.h"
#include <QtScript/QScriptEngine>
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include "script_point.h"
#include "helper_api.h"
#include "../vision/vision_api.h"
#include "../browser/browser_api.h"
#include "../browser/browser.h"
#include "../android/android_api.h"
#include "../desktop/desktop_api.h"

Bot::Bot(QString script_path)
	: m_script_path(script_path)
{}

void Bot::runScript()
{
	running = true;
	emit this->started();
	
	// Reset settings eventually made by a previously run script.
	Browser::unmodifyResources();
	
	// Create script engine and enable our api in it's global context.
	script_engine = new QScriptEngine(this);
	HelperAPI::enable(this, script_engine);
	VisionAPI::enable(this, script_engine);
	BrowserAPI::enable(this, script_engine);
	AndroidAPI::enable(this, script_engine);
	DesktopAPI::enable(this, script_engine);
	
	// Register custom QObject based types.
	qScriptRegisterMetaType(script_engine, Image::toScriptValue, Image::fromScriptValue);
	qScriptRegisterMetaType(script_engine, HSVColor::toScriptValue, HSVColor::fromScriptValue);
	qScriptRegisterMetaType(script_engine, BlobTpl::toScriptValue, BlobTpl::fromScriptValue);
	qScriptRegisterMetaType(script_engine, Match::toScriptValue, Match::fromScriptValue);
	qScriptRegisterMetaType(script_engine, ScriptPoint::toScriptValue, ScriptPoint::fromScriptValue);
	
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
	
	// Run the script and clean up after doing so.
	// NOTE: Putting this in a try-catch statement does nothing.
	// I threw a exception in the browser api and the program crashed.
	QScriptValue result = script_engine->evaluate(contents, this->m_script_path);
	script_engine->collectGarbage();
	
	// Check whether the script ended due to errors. If so print them to the users log.
	if (result.isError()) {
		QString debug_msg = QString("<b>Uncaught exception</b> at line %1 : %2")
			.arg(result.property("lineNumber").toString()).arg(result.toString());
		emit this->message(debug_msg, true, true);
	} else {
		// Script execution/evaluation ended successfully.
		emit this->message("Bot script execution finished.", true);
	}
	
	running = false;
	emit stopped(!result.isError());
}

bool Bot::isRunning() const {
	return running;
}

void Bot::stop()
{
	script_engine->abortEvaluation();
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