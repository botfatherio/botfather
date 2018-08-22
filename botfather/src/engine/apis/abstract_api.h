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
	void enable(QScriptEngine *engine, QString api_name);
	
protected:
	Bot *bot() const;
	
	// Template method
	virtual void extendGlobalApiObject(QScriptEngine *engine, QScriptValue &api_object);
	
private:
	Bot *bot_p;
};

#define REGISTER_API(ENGINE_P, BOT_P, API_CLS, API_NAME) \
{ \
	API_CLS *api_instance = new API_CLS(BOT_P, ENGINE_P); \
	api_instance->enable(ENGINE_P, API_NAME); \
}

#endif // BFP_ENGINE_APIS_ABSTRACT_API_H