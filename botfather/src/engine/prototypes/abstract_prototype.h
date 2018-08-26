#ifndef BFP_ENGINE_PROTOTYPES_ABSTRACT_PROTOTYPE_H
#define BFP_ENGINE_PROTOTYPES_ABSTRACT_PROTOTYPE_H

#include <QObject>
#include <QScriptable>
#include <QScriptEngine>

class AbstractPrototype : public QObject, public QScriptable
{
	Q_OBJECT
	
public:
	AbstractPrototype(QObject *parent = nullptr);
	
	// Inheriting classes must implement a static constructor method with this signature:
	// static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine)
	// Some notes about it:
	// Note: Returning context->thisObject(); when context->isCalledAsConstructor() is true
	// (as recommended by Qt) leaves us with an qobject that can't be casted to QSize anymore.
	// Note: Returning a value here allows us to cast the resulting QScriptValue to both QSize values AND pointers!
	
	// Note about getter and setter methods:
	// Since the data is not to be modified, it's OK to cast to a value instead of a pointer.
	
	Q_INVOKABLE virtual QString toString() const = 0;
	
	static bool isQColor(QScriptValue value);
	static bool isQPoint(QScriptValue value);
	static bool isQSize(QScriptValue value);
	static bool isQRect(QScriptValue value);
	
	static QColor toQColor(QScriptValue value);
	static QSize toQSize(QScriptValue value);
};

#define REGISTER_PROTO(ENGINE_PTR, PROTO_CLS, TYPE_CLS, TYPE_CLS_NAME) \
{ \
	PROTO_CLS *prototype = new PROTO_CLS(ENGINE_PTR); \
	QScriptValue js_prototype = ENGINE_PTR->newQObject(prototype); \
	QScriptValue js_constructor = ENGINE_PTR->newFunction(PROTO_CLS::constructor, js_prototype); \
	ENGINE_PTR->setDefaultPrototype(qMetaTypeId<TYPE_CLS>(), js_prototype); \
	ENGINE_PTR->setDefaultPrototype(qMetaTypeId<TYPE_CLS*>(), js_prototype); \
	ENGINE_PTR->globalObject().setProperty(QString(TYPE_CLS_NAME), js_constructor); \
}

#define NO_MATCHING_CTOR(cls_name, docs_url) \
	QString message = QString("No matching constructor for initialization of '%1'. %2").arg(QString(cls_name)).arg(QString(docs_url)); \
	return context->throwError(QScriptContext::SyntaxError, message);

#define RANGE_ERROR(CONTEXT, TEXT) CONTEXT->throwError(QScriptContext::RangeError, QString(TEXT));

#endif // BFP_ENGINE_PROTOTYPES_ABSTRACT_PROTOTYPE_H
