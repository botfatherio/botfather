#include "abstract_prototype.h"
#include <QColor>
#include <QSize>

AbstractPrototype::AbstractPrototype(QObject *parent)
	: QObject(parent)
{
	
}

bool AbstractPrototype::isQColor(QScriptValue value)
{
	return value.toVariant().canConvert(qMetaTypeId<QColor>());
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

QColor AbstractPrototype::toQColor(QScriptValue value)
{
	return qscriptvalue_cast<QColor>(value);
}

QSize AbstractPrototype::toQSize(QScriptValue value)
{
	return qscriptvalue_cast<QSize>(value);
}