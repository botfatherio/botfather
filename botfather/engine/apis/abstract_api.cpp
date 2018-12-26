#include "abstract_api.h"

AbstractAPI::AbstractAPI(Engine *bot, QObject *parent)
	: QObject(parent)
	, bot_p(bot)
{
	
}

Engine* AbstractAPI::bot() const
{
	return bot_p;
}

void AbstractAPI::extendGlobalApiObject(QScriptEngine *, QScriptValue &)
{

}

void AbstractAPI::enable(QScriptEngine *engine, QString api_name)
{
	// Note: The engine pointer argument is required because engine() returns a nullptr if the Qt function was not invoked from script code.
	QScriptValue api_js_obj(engine->newQObject(
		this,
		QScriptEngine::ScriptOwnership,
		QScriptEngine::SkipMethodsInEnumeration | QScriptEngine::ExcludeSuperClassContents
	));
	extendGlobalApiObject(engine, api_js_obj);
	engine->globalObject().setProperty(api_name, api_js_obj);
}
