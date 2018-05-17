#ifndef BFP__VISION__BLOB_TPL_FACTORY_H
#define BFP__VISION__BLOB_TPL_FACTORY_H

#include <QObject>
#include <QString>
#include <QScriptEngine>
#include "hsv_color.h"

class BlobTplFactory : public QObject
{
	Q_OBJECT
	
public:
	BlobTplFactory(QScriptEngine* engine_p);
	static void enable(QScriptEngine* engine_p);
	static QScriptValue create(QScriptContext *context, QScriptEngine *engine);

private:
	QScriptEngine* m_engine_p;
};

#endif // BFP__VISION__BLOB_TPL_FACTORY_H