#include "rect_prototype.h"

#define THIS_RECT() qscriptvalue_cast<QRect>(thisObject())
#define THIS_RECT_P() qscriptvalue_cast<QRect*>(thisObject())

QScriptValue RectPrototype::constructor(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() == 0)
	{
		return engine->toScriptValue(QRect());
	}
	
	if (context->argumentCount() == 2 && isQPoint(context->argument(0)) && isQSize(context->argument(1)))
	{
		QPoint top_left = qscriptvalue_cast<QPoint>(context->argument(0));
		QSize size = qscriptvalue_cast<QSize>(context->argument(1));
		return engine->toScriptValue(QRect(top_left, size));
	}
	
	if (context->argumentCount() == 2 && isQPoint(context->argument(0)) && isQPoint(context->argument(1)))
	{
		QPoint top_left = qscriptvalue_cast<QPoint>(context->argument(0));
		QPoint bottom_right = qscriptvalue_cast<QPoint>(context->argument(1));
		return engine->toScriptValue(QRect(top_left, bottom_right));
	}
	
	if (context->argumentCount() == 4 && context->argument(0).isNumber() && context->argument(1).isNumber() && context->argument(2).isNumber() && context->argument(3).isNumber())
	{
		int left = static_cast<int>(context->argument(0).toNumber());
		int top = static_cast<int>(context->argument(1).toNumber());
		int width = static_cast<int>(context->argument(2).toNumber());
		int height = static_cast<int>(context->argument(3).toNumber());
		return engine->toScriptValue(QRect(left, top, width, height));
	}
	
	NO_MATCHING_CTOR("Rect", RECT_PROTOTYPE_DOCS)
}

bool RectPrototype::isEmpty() const
{
	return THIS_RECT().isEmpty();
}

int RectPrototype::getTop() const
{
	return THIS_RECT().top();
}

int RectPrototype::getLeft() const
{
	return THIS_RECT().left();
}

int RectPrototype::getRight() const
{
	return THIS_RECT().right();
}

int RectPrototype::getBottom() const
{
	// Note that for historical reasons this QRect::bottom() returns QRect::top() + QRect::height() - 1;
	// use QRect::y() + QRect::height() to retrieve the true y-coordinate.
	QRect rect = THIS_RECT();
	return rect.top() + rect.height();
}

QPoint RectPrototype::getCenter() const
{
	return THIS_RECT().center();
}

QPoint RectPrototype::getTopLeft() const
{
	return THIS_RECT().topLeft();
}

QPoint RectPrototype::getTopRight() const
{
	return THIS_RECT().topRight();
}

QPoint RectPrototype::getBottomLeft() const
{
	return THIS_RECT().bottomLeft();
}

QPoint RectPrototype::getBottomRight() const
{
	return THIS_RECT().bottomRight();
}

int RectPrototype::getWidth() const
{
	return THIS_RECT().width();
}

void RectPrototype::setWidth(int width)
{
	THIS_RECT_P()->setWidth(width);
}

int RectPrototype::getHeight() const
{
	return THIS_RECT().height();
}

void RectPrototype::setHeight(int height)
{
	THIS_RECT_P()->setHeight(height);
}

QSize RectPrototype::getSize() const
{
	return THIS_RECT().size();
}

void RectPrototype::moveTop(int y)
{
	THIS_RECT_P()->moveTop(y);
}

void RectPrototype::moveLeft(int x)
{
	THIS_RECT_P()->moveTop(x);
}

void RectPrototype::moveRight(int x)
{
	THIS_RECT_P()->moveTop(x);
}

void RectPrototype::moveBottom(int y)
{
	THIS_RECT_P()->moveTop(y);
}

bool RectPrototype::contains(const QPoint &point, bool proper) const
{
	return THIS_RECT().contains(point, proper);
}

bool RectPrototype::contains(const QRect &other_rect, bool proper) const
{
	return THIS_RECT().contains(other_rect, proper);
}

bool RectPrototype::intersects(const QRect &other_rect) const
{
	return THIS_RECT().intersects(other_rect);
}

QRect RectPrototype::united(const QRect &other_rect) const
{
	return THIS_RECT().united(other_rect);
}

QString RectPrototype::toString() const
{
	QRect rect = THIS_RECT();
	return QString("Rect(left: %1, right: %2, width: %3, height: %4)").arg(rect.left()).arg(rect.top()).arg(rect.width()).arg(rect.height());
}
