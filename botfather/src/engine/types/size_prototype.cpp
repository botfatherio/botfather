#include "size_prototype.h"
#include <QDebug>

QScriptValue SizePrototype::constructor(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() == 0)
	{
		return engine->toScriptValue(QSize());
	}
	
	if (context->argumentCount() == 2 && context->argument(0).isNumber() && context->argument(1).isNumber())
	{
		int width = context->argument(0).toInt32();
		int height = context->argument(1).toInt32();
		
		return engine->toScriptValue(QSize(width, height));
	}
	
	NO_MATCHING_CTOR("Size", SIZE_PROTOTYPE_DOCS)
}

int SizePrototype::getWidth() const
{
	QSize size = qscriptvalue_cast<QSize>(thisObject());
	return size.width();
}

void SizePrototype::setWidth(int width)
{
	QSize *size = qscriptvalue_cast<QSize*>(thisObject());
	if (size) { size->setWidth(width); }
}

int SizePrototype::getHeight() const
{
	QSize size = qscriptvalue_cast<QSize>(thisObject());
	return size.height();
}

void SizePrototype::setHeight(int height)
{
	QSize *size = qscriptvalue_cast<QSize*>(thisObject());
	if (size) { size->setHeight(height); }
}

QString SizePrototype::toString() const
{
	QSize size = qscriptvalue_cast<QSize>(thisObject());
	return QString("%1x%2").arg(size.width()).arg(size.height());
}