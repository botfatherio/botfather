#include "margins_prototype.h"

#define THIS_MARGINS() qscriptvalue_cast<QMargins>(thisObject())
#define THIS_MARGINS_P() qscriptvalue_cast<QMargins*>(thisObject())

QScriptValue MarginsPrototype::constructor(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() == 0) {
		return engine->toScriptValue(QMargins());
	}

	// new Margins(left, right, top, bottom). Thats different that Qts order...
	if (context->argumentCount() == 4 && context->argument(0).isNumber() && context->argument(1).isNumber()
		&& context->argument(2).isNumber() && context->argument(3).isNumber())
	{
		int left = context->argument(0).toInt32();
		int right = context->argument(1).toInt32();
		int top = context->argument(2).toInt32();
		int bottom = context->argument(3).toInt32();
		return engine->toScriptValue(QMargins(left, right, top, bottom));
	}

	NO_MATCHING_CTOR("Margins", MARGINS_PROTOTYPE_DOCS)
}

bool MarginsPrototype::isNull() const
{
	return THIS_MARGINS().isNull();
}

int MarginsPrototype::getTop() const
{
	return THIS_MARGINS().top();
}

void MarginsPrototype::setTop(int top)
{
	THIS_MARGINS_P()->setTop(top);
}

int MarginsPrototype::getLeft() const
{
	return THIS_MARGINS().left();
}

void MarginsPrototype::setLeft(int left)
{
	THIS_MARGINS_P()->setLeft(left);
}

int MarginsPrototype::getRight() const
{
	return THIS_MARGINS().right();
}

void MarginsPrototype::setRight(int right)
{
	THIS_MARGINS_P()->setRight(right);
}

int MarginsPrototype::getBottom() const
{
	return THIS_MARGINS().bottom();
}

void MarginsPrototype::setBottom(int bottom)
{
	THIS_MARGINS_P()->setBottom(bottom);
}

QString MarginsPrototype::toString() const
{
	QMargins margins = THIS_MARGINS();
	return QString("Margins(left: %1, right: %2, top: %3, bottom: %4)").arg(margins.left()).arg(margins.right()).arg(margins.top()).arg(margins.bottom());
}
