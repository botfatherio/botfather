#ifndef SCRIPTING__SCRIPT_POINT_H
#define SCRIPTING__SCRIPT_POINT_H

#include <QObject>
#include <QScriptEngine>

class ScriptPoint : public QObject
{
	Q_OBJECT
public:
	ScriptPoint();
	explicit ScriptPoint(int x, int y);
	
	static QScriptValue toScriptValue(QScriptEngine *engine, ScriptPoint* const &in);
	static void fromScriptValue(const QScriptValue &object, ScriptPoint* &out);
	
	Q_INVOKABLE int getX() const;
	Q_INVOKABLE int getY() const;
	Q_INVOKABLE void setX(int x);
	Q_INVOKABLE void setY(int y);
	
private:
	int m_x = 0;
	int m_y = 0;
};

Q_DECLARE_METATYPE(ScriptPoint*)

#endif // SCRIPTING__SCRIPT_POINT_H