#include "engine.h"
#include <QScriptEngine>
#include <QRegularExpression>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTime>
#include "prototypes/blob_tpl_prototype.h"
#include "prototypes/margins_prototype.h"
#include "prototypes/point_prototype.h"
#include "prototypes/image_prototype.h"
#include "prototypes/color_prototype.h"
#include "prototypes/match_prototype.h"
#include "prototypes/timer_prototype.h"
#include "prototypes/size_prototype.h"
#include "prototypes/rect_prototype.h"
#include "prototypes/url_prototype.h"
#include "prototypes/browser_prototype.h"
#include "apis/helper_api.h"
#include "apis/vision_api.h"
#include "apis/android_api.h"
#include "apis/desktop_api.h"
#include "apis/algorithm_api.h"
#include "apis/config_api.h"

Engine::Engine(const QString &id, QString script_path, const QString &config_ini_path)
	: m_id(id)
	, script_path(script_path)
	, m_config_ini_path(config_ini_path)
{
	// Otherwise Bot::LogSource can't be used with slots
    qRegisterMetaType<Engine::LogSource>("Engine::LogSource");

	// The script engines parent MUST BE the bot instance. This way we can obtain a pointer to the bot
	// instance in static functions (like constructors) by casting the engines parent to a Bot*.
	script_engine = new QScriptEngine(this);
}

Engine::~Engine()
{
	// The QJSEngine (not QScriptEngine) docs state that the garbage is not collected
	// when the engine gets destroyed. The advise to call collectGarbage before deleting it.
	script_engine->collectGarbage();
	//delete script_engine; // has bot as parent.
}

QString Engine::id() const
{
	return m_id;
}

QString Engine::getAbsoluteScriptDirPath()
{
	QFileInfo file_info(script_path);
	return file_info.dir().absolutePath();
}

QString Engine::normalisePath(QString path)
{
	QFileInfo file_info(path);
	if (file_info.isRelative()) {
		return QDir(getAbsoluteScriptDirPath()).filePath(path);
	}
	return path;
}

bool Engine::scriptFileExists(QString file_path)
{
	file_path = normalisePath(file_path);
	QFileInfo file_info(file_path);
	return file_info.exists() && file_info.isFile();
}

QString Engine::configIniPath() const
{
	return m_config_ini_path;
}

void Engine::runScript()
{
	emit started();

#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
    qsrand(static_cast<uint>(QTime::currentTime().msec()));
#endif

	REGISTER_API(script_engine, this, AndroidAPI, "Android");
	REGISTER_API(script_engine, this, DesktopAPI, "Desktop");
	REGISTER_API(script_engine, this, HelperAPI, "Helper");
	REGISTER_API(script_engine, this, VisionAPI, "Vision");
	REGISTER_API(script_engine, this, AlgorithmAPI, "Algorithm");
	REGISTER_API(script_engine, this, ConfigAPI, "Config");
	
	REGISTER_PROTO(script_engine, TimerPrototype, QElapsedTimer, "Timer");
	REGISTER_PROTO(script_engine, BlobTplPrototype, BlobTpl, "BlobTpl");
	REGISTER_PROTO(script_engine, MarginsPrototype, QMargins, "Margins");
	REGISTER_PROTO(script_engine, PointPrototype, QPoint, "Point");
	REGISTER_PROTO(script_engine, ImagePrototype, QImage, "Image");
	REGISTER_PROTO(script_engine, ColorPrototype, QColor, "Color");
	REGISTER_PROTO(script_engine, MatchPrototype, Match, "Match");
	REGISTER_PROTO(script_engine, SizePrototype, QSize, "Size");
	REGISTER_PROTO(script_engine, RectPrototype, QRect, "Rect");
	REGISTER_PROTO(script_engine, UrlPrototype, QUrl, "Url");
	REGISTER_PROTO(script_engine, BrowserPrototype, Browser, "Browser");

	// Try to open the submitted script file.
	QFile script_file(script_path);
	if (!script_file.open(QIODevice::ReadOnly)) {
		QString debug_msg("Can't read the contents from " + script_path + ". Please check the files permissions.");
		emit log(debug_msg, LogSource::System);
		emit stopped(false);
		return;
	}
	
	// Get the script files contents.
	QByteArray script = script_file.readAll();
	script_file.close();
	emit log("Executing bot script " + script_path, LogSource::System);
	
	// Run the script and clean up after doing so.
	// NOTE: Putting this in a try-catch statement does nothing.
	// I threw a exception in the browser api and the program crashed.
	QScriptValue result(script_engine->evaluate(script, script_path));
	
	// Check whether the script ended due to errors. If so print them to the users log.
	if (result.isError()) {
		QString error_msg = replaceQtWithEngineTypeNames(result.toString());
		QString debug_msg = QString("<b>Uncaught exception</b> at line %1 : %2").arg(result.property("lineNumber").toString()).arg(error_msg);
		emit log(debug_msg, LogSource::Error);
	} else {
		emit log("Bot script execution finished.", LogSource::System);
	}

	// Close all unnamed/non-persistent browsers of this session
	BrowserHost::instance()->closeManagedTemporaryBrowsers(id());

	emit stopped(!result.isError());
}

void Engine::stop()
{
	script_engine->abortEvaluation();
}

QString Engine::replaceQtWithEngineTypeNames(QString text)
{
	// Ignore reference errors: "ReferenceError: Can't find variable: QSomethingThatDoesNotExist"
	if (text.contains("ReferenceError: Can't find variable"))
	{
		return text;
	}

	// https://doc.qt.io/archives/qt-5.10/qtscript-index.html#conversion-between-qt-script-and-c-types
	// https://developer.mozilla.org/de/docs/Web/JavaScript/Reference/Global_Objects/String
	// Note: Replace pointer types before non pointer types. Otherwise the "*" will not be replaced.

	// Detect "QTypeName" and "QTypeName*" and make "TypeName" be in the first capture group.
	// The first capture group can be refered to with "\\1".

	QRegularExpression qtype_re("Q([A-Z][a-zA-Z])+(\\*?)");
	text.replace(qtype_re, "\\1");
	return text;
}
