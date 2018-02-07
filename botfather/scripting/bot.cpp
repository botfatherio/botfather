#include "bot.h"
#include <QJSEngine>
#include <QFile>
#include <QDebug>
#include "../scripting/helper_api.h"
#include "../vision/vision_api.h"
#include "../browser/browser_api.h"
#include "../browser/browser.h"

Bot::Bot(BotThread* thread_p, QString script_path) : m_thread_p(thread_p), m_script_path(script_path)
{}

void Bot::runScript()
{
	emit this->started();
	
	// Reset settings eventually made by a previously run script.
	Browser::unmodifyRessources();
	
	// Create js engine and enable our api in it's global context.
	QJSEngine engine;
	HelperAPI::enable(m_thread_p, this, &engine);
	VisionAPI::enable(&engine);
	BrowserAPI::enable(&engine);
	
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
	QJSValue result = engine.evaluate(contents, this->m_script_path);
	
	// Check whether the script ended due to errors. If so print them to the users log.
	if (result.isError()) {
		QString debug_msg("<b style='color:red'>Uncaught exception</b> at line " + result.property("lineNumber").toString() + ": " + result.toString());
		emit this->message(debug_msg, true);
		emit this->stopped(false);
		return;
	}

	// Script execution/evaluation ended successfully.
	emit this->message("Bot script execution finished.", true);
	emit this->stopped(true);
}