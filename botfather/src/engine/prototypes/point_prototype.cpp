#include "point_prototype.h"

#define THIS_POINT() qscriptvalue_cast<QPoint>(thisObject())
#define THIS_POINT_P() qscriptvalue_cast<QPoint*>(thisObject())

QScriptValue PointPrototype::constructor(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() == 0) {
		return engine->toScriptValue(QPoint());
	}
	
	if (context->argumentCount() == 2 && context->argument(0).isNumber() && context->argument(1).isNumber())
	{
		int x = context->argument(0).toInt32();
		int y = context->argument(1).toInt32();
		return engine->toScriptValue(QPoint(x, y));
	}
	
	NO_MATCHING_CTOR("Point", POINT_PROTOTYPE_DOCS)
}

int PointPrototype::getX() const
{
	return THIS_POINT().x();
}

void PointPrototype::setX(int x)
{
	THIS_POINT_P()->setX(x);
}

int PointPrototype::getY() const
{
	return THIS_POINT().y();
}

void PointPrototype::setY(int y)
{
	THIS_POINT_P()->setY(y);
}

int PointPrototype::dotProduct(QPoint other_point) const
{
	return QPoint::dotProduct(THIS_POINT(), other_point);
}

int PointPrototype::manhattanLength() const
{
	return THIS_POINT().manhattanLength();
}

QString PointPrototype::toString() const
{
	QPoint point = THIS_POINT();
	return QString("Point(x: %1, y: %2)").arg(point.x()).arg(point.y());
}