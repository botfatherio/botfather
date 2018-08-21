#ifndef BFP_ENGINE_APIS_ABSTRACT_API_H
#define BFP_ENGINE_APIS_ABSTRACT_API_H

#include <QObject>
#include <QScriptable>
#include <QScriptEngine>
#include "../bot.h"

class AbstractAPI : public QObject, public QScriptable
{
	Q_OBJECT
	
public:
	AbstractAPI(Bot *bot, QObject *parent = nullptr);
	
protected:
	Bot *bot() const;
	
private:
	Bot *bot_p;
};

#define REGISTER_API(ENGINE_P, BOT_P, API_CLS, API_NAME) \
{ \
	API_CLS *api_instance = new API_CLS(BOT_P, ENGINE_P); \
	QScriptValue api_js_obj = ENGINE_P->newQObject( \
		api_instance, \
		QScriptEngine::ScriptOwnership, \
		QScriptEngine::SkipMethodsInEnumeration | QScriptEngine::ExcludeSuperClassContents \
	); \
	ENGINE_P->globalObject().setProperty(API_NAME, api_js_obj); \
}

#endif // BFP_ENGINE_APIS_ABSTRACT_API_H