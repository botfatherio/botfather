#include "script_point_factory.h"
#include "script_point.h"

ScriptPointFactory::ScriptPointFactory(QScriptEngine *engine_p, QObject *parent)
	: QObject(parent), m_engine_p(engine_p)
{}

void ScriptPointFactory::enable(QScriptEngine *engine_p)
{
	engine_p->globalObject().setProperty("Point", engine_p->newFunction(ScriptPointFactory::create));
}

QScriptValue ScriptPointFactory::create(QScriptContext *context, QScriptEngine *engine)
{
	// Default x and y coordinates.
	int x = -1;
	int y = -1;
	
	// A ScriptPoint can either be constructed providing x and y or no parameters.
	if (context->argumentCount() == 2) {
		if (!context->argument(0).isNumber() || !context->argument(1).isNumber()){
			return context->throwError("Both x and y must be numbers.");
		}
		x = static_cast<int>(context->argument(0).toNumber());
		y = static_cast<int>(context->argument(1).toNumber());
	}
	else if (context->argumentCount() != 0) {
		return context->throwError("Either 2 or no arguments were expected.");
	}
	
	return engine->toScriptValue(new ScriptPoint(x, y));
}