#ifndef ENGINE_TYPES_COLOR_PROTOTYPE_H
#define ENGINE_TYPES_COLOR_PROTOTYPE_H
#define COLOR_PROTOTYPE_DOCS "https://botfather.io/docs/apiref/color/"

#include <QColor>
#include "abstract_prototype.h"

Q_DECLARE_METATYPE(QColor)
Q_DECLARE_METATYPE(QColor*)

class ColorPrototype : public AbstractPrototype
{
	Q_OBJECT
	
public:
	static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
	using AbstractPrototype::AbstractPrototype;
	
	Q_INVOKABLE int getRed() const;
	Q_INVOKABLE void setRed(int red);
	
	Q_INVOKABLE int getGreen() const;
	Q_INVOKABLE void setGreen(int green);
	
	Q_INVOKABLE int getBlue() const;
	Q_INVOKABLE void setBlue(int blue);
	
	Q_INVOKABLE QString toString() const;
};

#endif // ENGINE_TYPES_COLOR_PROTOTYPE_H
