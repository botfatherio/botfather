#include "script_point.h"

ScriptPoint::ScriptPoint()
{
	
}

ScriptPoint::ScriptPoint(int x, int y) : m_x(x), m_y(y)
{
	
}

QScriptValue ScriptPoint::toScriptValue(QScriptEngine *engine, ScriptPoint * const &in)
{
	return engine->newQObject(in);
}

void ScriptPoint::fromScriptValue(const QScriptValue &object, ScriptPoint *&out)
{
	out = qobject_cast<ScriptPoint*>(object.toQObject());
}

int ScriptPoint::getX() const
{
	return m_x;
}

int ScriptPoint::getY() const
{
	return m_y;
}

void ScriptPoint::setY(int y)
{
	m_y = y;
}

void ScriptPoint::setX(int x)
{
	m_x = x;
}