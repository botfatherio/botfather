#include "hsv_color_factory.h"
#include "hsv_color.h"

HSVColorFactory::HSVColorFactory(QScriptEngine *engine_p)
	: m_engine_p(engine_p)
{}

QScriptValue HSVColorFactory::createInstance(int h, int s, int v)
{
	// The engine takes ownership and destroys the object if no longer required.
	return m_engine_p->newQObject(new HSVColor(h, s, v));
}

void HSVColorFactory::enable(QScriptEngine *engine_p)
{
	engine_p->globalObject().setProperty("HSVColor", engine_p->newFunction(HSVColorFactory::create));
}

QScriptValue HSVColorFactory::create(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() != 3) {
		return context->throwError(QString("To many/few arguments. (Expected 3, got %1).").arg(context->argumentCount()));
	}
	if (!context->argument(0).isNumber() || !context->argument(1).isNumber() || !context->argument(2).isNumber()) {
		return context->throwError("All arguments must be numbers.");
	}
	
	int h = context->argument(0).toNumber();
	int s = context->argument(1).toNumber();
	int v = context->argument(2).toNumber();
	
	if (h < 0 || h > 360) {
		return context->throwError("Argument h must be between 0 and 360°.");
	}
	if (s < 0 || s > 100) {
		return context->throwError("Argument s must be between 0 and 100%.");
	}
	if (v < 0 || v > 100) {
		return context->throwError("Argument v must be between 0 and 100%.");
	}
	
	return engine->toScriptValue(new HSVColor(h, s, v));
}