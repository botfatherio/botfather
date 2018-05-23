#include "point.h"

Point::Point()
{
	
}

Point::Point(int x, int y) : m_x(x), m_y(y)
{
	
}

QScriptValue Point::toScriptValue(QScriptEngine *engine, Point * const &in)
{
	return engine->newQObject(in);
}

void Point::fromScriptValue(const QScriptValue &object, Point *&out)
{
	out = qobject_cast<Point*>(object.toQObject());
}

int Point::getX() const
{
	return m_x;
}

int Point::getY() const
{
	return m_y;
}

void Point::setY(int y)
{
	m_y = y;
}

void Point::setX(int x)
{
	m_x = x;
}