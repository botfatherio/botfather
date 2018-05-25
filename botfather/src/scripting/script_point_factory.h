#ifndef SCRIPTING__SCRIPT_POINT_FACTORY_H
#define SCRIPTING__SCRIPT_POINT_FACTORY_H

#include <QObject>
#include <QScriptEngine>

class ScriptPointFactory : public QObject
{
	Q_OBJECT

public:
	ScriptPointFactory(QScriptEngine* engine_p, QObject *parent = nullptr);
	static void enable(QScriptEngine* engine_p);
	static QScriptValue create(QScriptContext *context, QScriptEngine *engine);

private:
	QScriptEngine* m_engine_p;
};

#endif // SCRIPTING__SCRIPT_POINT_FACTORY_H