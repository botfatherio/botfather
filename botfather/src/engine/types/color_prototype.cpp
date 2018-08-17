#include "color_prototype.h"
#include <QHash>

static QHash<QString, QColor> color_map = {
	{ "black", Qt::black },
	{ "blue", Qt::blue },
	{ "cyan", Qt::cyan },
	{ "darkblue", Qt::darkBlue },
	{ "darkcyan", Qt::darkCyan },
	{ "darkgray", Qt::darkGray },
	{ "darkgreen", Qt::darkGreen },
	{ "darkmagenta", Qt::darkMagenta },
	{ "darkred", Qt::darkRed },
	{ "darkyellow", Qt::darkYellow },
	{ "gray", Qt::gray },
	{ "green", Qt::green },
	{ "lightgray", Qt::lightGray },
	{ "magenta", Qt::magenta },
	{ "red", Qt::red },
	{ "transparent", Qt::transparent },
	{ "white", Qt::white },
	{ "yellow", Qt::yellow },
};

QScriptValue ColorPrototype::constructor(QScriptContext *context, QScriptEngine *engine)
{
	// Default constructor
	if (context->argumentCount() == 0) {
		return engine->toScriptValue(QColor());
	}
	
	// Construct by color name from color_map
	if (context->argumentCount() == 1 && context->argument(0).isString())
	{
		QString color_name = context->argument(0).toString().toLower();
		
		if (!color_map.contains(color_name))
		{
			QString message = QString("Unknown color '%1'. %2").arg(color_name).arg(COLOR_PROTOTYPE_DOCS);
			return context->throwError(QScriptContext::TypeError, message);
		}
		
		return engine->toScriptValue(color_map[color_name]);
	}
	
	// Construct from RGB value
	if (context->argumentCount() == 3 && context->argument(0).isNumber() && context->argument(1).isNumber() && context->argument(2).isNumber())
	{
		int r = context->argument(0).toInt32();
		int g = context->argument(1).toInt32();
		int b = context->argument(2).toInt32();
		
		if (r < 0 || g < 0 || b < 0 || r > 255 || g > 255 || b > 255)
		{
			return context->throwError(QScriptContext::RangeError, "r, g and b must be in range of 0-255");
		}
		
		return engine->toScriptValue(QColor(r, g, b));
	}
	
	NO_MATCHING_CTOR("Color", COLOR_PROTOTYPE_DOCS)
}

int ColorPrototype::getRed() const
{
	QColor color = qscriptvalue_cast<QColor>(thisObject());
	return color.red();
}

void ColorPrototype::setRed(int red)
{
	QColor *color = qscriptvalue_cast<QColor*>(thisObject());
	if (color) { color->setRed(red); }
}

int ColorPrototype::getGreen() const
{
	QColor color = qscriptvalue_cast<QColor>(thisObject());
	return color.green();
}

void ColorPrototype::setGreen(int green)
{
	QColor *color = qscriptvalue_cast<QColor*>(thisObject());
	if (color) { color->setGreen(green); }
}

int ColorPrototype::getBlue() const
{
	QColor color = qscriptvalue_cast<QColor>(thisObject());
	return color.blue();
}

void ColorPrototype::setBlue(int blue)
{
	QColor *color = qscriptvalue_cast<QColor*>(thisObject());
	if (color) { color->setBlue(blue); }
}

QString ColorPrototype::toString() const
{
	QColor color = qscriptvalue_cast<QColor>(thisObject());
	return QString("rgb(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue());
}