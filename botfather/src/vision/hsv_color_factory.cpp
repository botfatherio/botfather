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

// static
void HSVColorFactory::enable(QScriptEngine *engine_p)
{
	QScriptValue hsv_color_factory = engine_p->newQObject(new HSVColorFactory(engine_p));
	engine_p->globalObject().setProperty("_hsv_color_factory", hsv_color_factory);
	engine_p->evaluate(
		"function HSVColor(h, s, v)"
		"{"
			"if (typeof(h)==='undefined') h = 0;"
			"if (typeof(s)==='undefined') s = 0;"
			"if (typeof(v)==='undefined') v = 0;"
			"return _hsv_color_factory.createInstance(h, s, v);"
		"}"
	);
}