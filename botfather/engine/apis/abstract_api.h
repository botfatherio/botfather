#ifndef BFP_ENGINE_APIS_ABSTRACT_API_H
#define BFP_ENGINE_APIS_ABSTRACT_API_H

#include <QObject>
#include <QScriptable>
#include <QScriptEngine>
#include "../engine.h"

class AbstractAPI : public QObject, public QScriptable
{
	Q_OBJECT
	
public:
	AbstractAPI(Engine *bot, QObject *parent = nullptr);
	void enable(QScriptEngine *engine, QString api_name);
	
protected:
	Engine *bot() const;
	
	// Template method
	virtual void extendGlobalApiObject(QScriptEngine *engine, QScriptValue &api_object);
	
private:
	Engine *bot_p;
};

#define REGISTER_API(ENGINE_P, BOT_P, API_CLS, API_NAME) \
{ \
	API_CLS *api_instance = new API_CLS(BOT_P, ENGINE_P); \
	api_instance->enable(ENGINE_P, API_NAME); \
}

#define MB_ARRAY(object, object_name) \
{ \
	if (!matches.isArray()) { \
		QString message = QString("%1 must be an array.").arg(QString(object_name)); \
	    return engine()->currentContext()->throwError(QScriptContext::TypeError, message); \
    } \
}

#define MB_NOT_NULL(object, object_name) \
{ \
	if (object.isNull()) { \
		QString message = QString("%1 must not be null.").arg(QString(object_name)); \
		return engine()->currentContext()->throwError(QScriptContext::TypeError, message); \
	} \
}

#define MB_SMALLER(image1, image2, image1_name, image2_name) \
{ \
	if (image1.height() >= image2.height() || image1.width() >= image2.width()) { \
		QString message = QString("%1 must be smaller than %2.").arg(QString(image1_name)).arg(QString(image2_name)); \
		return engine()->currentContext()->throwError(QScriptContext::TypeError, message); \
	} \
}

#define MB_SAME_SIZE(image1, image2, image1_name, image2_name) \
{ \
	if (image1.height() != image2.height() || image1.width() != image2.width()) { \
		QString message = QString("%1 and %2 must have the same size.").arg(QString(image1_name)).arg(QString(image2_name)); \
		return engine()->currentContext()->throwError(QScriptContext::TypeError, message); \
	} \
}

#define MB_FOUND(match, match_name) \
{ \
	if (!match.isValid()) { \
		QString message = QString("%1 must have been found.").arg(QString(match_name)); \
		return engine()->currentContext()->throwError(QScriptContext::TypeError, message); \
	} \
}

#endif // BFP_ENGINE_APIS_ABSTRACT_API_H
