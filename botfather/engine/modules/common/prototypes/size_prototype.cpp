#include "size_prototype.hpp"

#define THIS_SIZE() qscriptvalue_cast<QSize>(thisObject())
#define THIS_SIZE_P() qscriptvalue_cast<QSize*>(thisObject())

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

bool SizePrototype::isEmpty() const
{
	return THIS_SIZE().isEmpty();
}

int SizePrototype::getWidth() const
{
	return THIS_SIZE().width();
}

void SizePrototype::setWidth(int width)
{
	THIS_SIZE_P()->setWidth(width);
}

int SizePrototype::getHeight() const
{
	return THIS_SIZE().height();
}

void SizePrototype::setHeight(int height)
{
	THIS_SIZE_P()->setHeight(height);
}

QSize SizePrototype::boundedTo(const QSize &other_size) const
{
	return THIS_SIZE().boundedTo(other_size);
}

QSize SizePrototype::expandedTo(const QSize &other_size) const
{
	return THIS_SIZE().expandedTo(other_size);
}

QSize SizePrototype::sizeAdded(const QSize &other_size) const
{
	return THIS_SIZE() + other_size;
}

QSize SizePrototype::sizeSubtracted(const QSize &other_size) const
{
	return THIS_SIZE() - other_size;
}

void SizePrototype::transpose()
{
	THIS_SIZE_P()->transpose();
}

QSize SizePrototype::transposed() const
{
	return THIS_SIZE().transposed();
}

QString SizePrototype::toString() const
{
	QSize size = THIS_SIZE();
	return QString("Size(width: %1, height: %2)").arg(size.width()).arg(size.height());
}
