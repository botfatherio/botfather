#ifndef BFP__VISION__HSV_COLOR_FACTORY_H
#define BFP__VISION__HSV_COLOR_FACTORY_H

#include <QObject>
#include <QString>
#include <QJSEngine>

class HSVColorFactory : public QObject
{
	Q_OBJECT

public:
	HSVColorFactory(QJSEngine* engine_p);
	Q_INVOKABLE QJSValue createInstance(int h, int s, int v);
	static void enable(QJSEngine* engine_p);

private:
	QJSEngine* m_engine_p;
};

#endif // BFP__VISION__HSV_COLOR_FACTORY_H