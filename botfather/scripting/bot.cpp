#include "bot.h"
#include <QJSEngine>
#include <QFile>
#include <QDebug>
#include "../vision/vision_api.h"

Bot::Bot(QString script_path) : m_script_path(script_path)
{}

void Bot::runScript()
{
	emit this->started();
	
	QJSEngine engine;
	engine.installExtensions(QJSEngine::ConsoleExtension);
	
	VisionAPI::enable(&engine);
	
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
	
	QJSValue result = engine.evaluate(contents, this->m_script_path);
	
	if (result.isError()) {
		QString debug_msg("<b style='color:red'>Uncaught exception</b> at line" + result.property("lineNumber").toString() + ":" + result.toString());
		emit this->message(debug_msg);
		emit this->stopped(false);
		return;
	}
	
	emit this->stopped(true);
}