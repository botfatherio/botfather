#ifndef ENGINE_PROTOTYPES_POINT_PROTOTYPE_H
#define ENGINE_PROTOTYPES_POINT_PROTOTYPE_H
#define POINT_PROTOTYPE_DOCS "https://botfather.io/docs/apiref/point/"

#include "abstract_prototype.h"
#include <QPoint>

class PointPrototype : public AbstractPrototype
{
	Q_OBJECT
	
public:
	static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
	using AbstractPrototype::AbstractPrototype;

	Q_INVOKABLE int getX() const;
	Q_INVOKABLE void setX(int width);
	
	Q_INVOKABLE int getY() const;
	Q_INVOKABLE void setY(int height);
	
	Q_INVOKABLE QString toString() const;
};

#endif // ENGINE_PROTOTYPES_POINT_PROTOTYPE_H