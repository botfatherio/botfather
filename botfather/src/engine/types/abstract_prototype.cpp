#include "abstract_prototype.h"

AbstractPrototype::AbstractPrototype(QObject *parent)
	: QObject(parent)
{
	
}

bool AbstractPrototype::isQPoint(QScriptValue value)
{
	return value.toVariant().canConvert(qMetaTypeId<QPoint>());
}

bool AbstractPrototype::isQSize(QScriptValue value)
{
	return value.toVariant().canConvert(qMetaTypeId<QSize>());
}

bool AbstractPrototype::isQRect(QScriptValue value)
{
	return value.toVariant().canConvert(qMetaTypeId<QRect>());
}