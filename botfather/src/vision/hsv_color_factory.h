#ifndef BFP__VISION__HSV_COLOR_FACTORY_H
#define BFP__VISION__HSV_COLOR_FACTORY_H

#include <QObject>
#include <QString>
#include <QScriptEngine>

class HSVColorFactory : public QObject
{
	Q_OBJECT

public:
	HSVColorFactory(QScriptEngine* engine_p);
	Q_INVOKABLE QScriptValue createInstance(int h, int s, int v);
	static void enable(QScriptEngine* engine_p);
	static QScriptValue create(QScriptContext *context, QScriptEngine *engine);

private:
	QScriptEngine* m_engine_p;
};

#endif // BFP__VISION__HSV_COLOR_FACTORY_H