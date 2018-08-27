#include "bot.h"
#include <QScriptEngine>
#include <QTextStream>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include "prototypes/blob_tpl_prototype.h"
#include "prototypes/point_prototype.h"
#include "prototypes/image_prototype.h"
#include "prototypes/color_prototype.h"
#include "prototypes/match_prototype.h"
#include "prototypes/size_prototype.h"
#include "prototypes/rect_prototype.h"
#include "apis/helper_api.h"
#include "apis/vision_api.h"
#include "apis/browser_api.h"
#include "apis/android_api.h"
#include "apis/desktop_api.h"
#include "modules/browser/browser.h"

Bot::Bot(QString script_path) : script_path(script_path)
{
	// The script engines parent MUST BE the bot instance. This way we can obtain a pointer to the bot
	// instance in static functions (like constructors) by casting the engines parent to a Bot*.
	script_engine = new QScriptEngine(this);
}

QString Bot::getAbsoluteScriptDirPath()
{
	QFileInfo file_info(script_path);
	return file_info.dir().absolutePath();
}

QString Bot::normalisePath(QString path)
{
	QFileInfo file_info(path);
	if (file_info.isRelative()) {
		return QDir(getAbsoluteScriptDirPath()).filePath(path);
	}
	return path;
}

bool Bot::scriptFileExists(QString file_path)
{
	file_path = normalisePath(file_path);
	QFileInfo file_info(file_path);
	return file_info.exists() && file_info.isFile();
}

void Bot::runScript()
{
	running = true;
	emit started();
	
	// Reset settings eventually made by a previously run script.
	Browser::unmodifyResources();
	
	REGISTER_API(script_engine, this, AndroidAPI, "Android");
	REGISTER_API(script_engine, this, BrowserAPI, "Browser");
	REGISTER_API(script_engine, this, DesktopAPI, "Desktop");
	REGISTER_API(script_engine, this, HelperAPI, "Helper");
	REGISTER_API(script_engine, this, VisionAPI, "Vision");
	
	REGISTER_PROTO(script_engine, BlobTplPrototype, BlobTpl, "BlobTpl");
	REGISTER_PROTO(script_engine, PointPrototype, QPoint, "Point");
	REGISTER_PROTO(script_engine, ImagePrototype, QImage, "Image");
	REGISTER_PROTO(script_engine, ColorPrototype, QColor, "Color");
	REGISTER_PROTO(script_engine, MatchPrototype, Match, "Match");
	REGISTER_PROTO(script_engine, SizePrototype, QSize, "Size");
	REGISTER_PROTO(script_engine, RectPrototype, QRect, "Rect");

	// Try to open the submitted script file.
	QFile script_file(script_path);
	if (!script_file.open(QIODevice::ReadOnly)) {
		QString debug_msg("Can't read the contents from " + script_path + ". Please check the files permissions.");
		emit message(debug_msg, true);
		emit stopped(false);
		return;
	}
	
	// Get the script files contents.
	QTextStream stream(&script_file);
	QString contents = stream.readAll();
	script_file.close();
	emit message("Executing bot script " + script_path, true);
	
	// Run the script and clean up after doing so.
	// NOTE: Putting this in a try-catch statement does nothing.
	// I threw a exception in the browser api and the program crashed.
	QScriptValue result = script_engine->evaluate(contents, script_path);
	script_engine->collectGarbage();
	
	// Check whether the script ended due to errors. If so print them to the users log.
	if (result.isError()) {
		QString error_msg = replaceQtWithEngineTypeNames(result.toString());
		QString debug_msg = QString("<b>Uncaught exception</b> at line %1 : %2").arg(result.property("lineNumber").toString()).arg(error_msg);
		emit message(debug_msg, true, true);
	} else {
		// Script execution/evaluation ended successfully.
		emit message("Bot script execution finished.", true);
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

QString Bot::replaceQtWithEngineTypeNames(QString text)
{
	// https://doc.qt.io/archives/qt-5.10/qtscript-index.html#conversion-between-qt-script-and-c-types
	// https://developer.mozilla.org/de/docs/Web/JavaScript/Reference/Global_Objects/String
	
	// Note: List "QType*" before "QType" otherwise "QType" will be replaced but the "*" stays.
	static QVector<QPair<QString, QString>> replacements = {
		{ QMetaType::typeName(qMetaTypeId<QColor*>()), "Color" },
		{ QMetaType::typeName(qMetaTypeId<QColor>()), "Color" },
		{ QMetaType::typeName(qMetaTypeId<QImage*>()), "Image" },
		{ QMetaType::typeName(qMetaTypeId<QImage>()), "Image" },
		{ QMetaType::typeName(qMetaTypeId<QPoint*>()), "Point" },
		{ QMetaType::typeName(qMetaTypeId<QPoint>()), "Point" },
		{ QMetaType::typeName(qMetaTypeId<QRect*>()), "Rect" },
		{ QMetaType::typeName(qMetaTypeId<QRect>()), "Rect" },
		{ QMetaType::typeName(qMetaTypeId<QSize*>()), "Size" },
		{ QMetaType::typeName(qMetaTypeId<QSize>()), "Size" },
		//{ QMetaType::typeName(qMetaTypeId<QString*>()), "String" },
		{ QMetaType::typeName(qMetaTypeId<QString>()), "String" },
	};
	for (QPair<QString, QString> pair : replacements) {
		text.replace(pair.first, pair.second);
	}
	return text;
}