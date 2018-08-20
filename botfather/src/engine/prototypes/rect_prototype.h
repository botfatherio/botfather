#ifndef ENGINE_PROTOTYPES_RECT_PROTOTYPE_H
#define ENGINE_PROTOTYPES_RECT_PROTOTYPE_H
#define RECT_PROTOTYPE_DOCS "https://botfather.io/docs/apiref/rect/"

#include "abstract_prototype.h"
#include <QRect>
#include <QPoint>

Q_DECLARE_METATYPE(QRect)
Q_DECLARE_METATYPE(QRect*)

class RectPrototype : public AbstractPrototype
{
	Q_OBJECT
	
public:
	static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
	using AbstractPrototype::AbstractPrototype;

	Q_INVOKABLE int getTop() const;
	Q_INVOKABLE void setTop(int top);
	
	Q_INVOKABLE int getLeft() const;
	Q_INVOKABLE void setLeft(int left);
	
	Q_INVOKABLE int getRight() const;
	Q_INVOKABLE void setRight(int right);
	
	Q_INVOKABLE int getBottom() const;
	Q_INVOKABLE void setBottom(int bottom);
	
	Q_INVOKABLE QPoint getTopLeft() const;
	Q_INVOKABLE QPoint getTopRight() const;
	Q_INVOKABLE QPoint getBottomLeft() const;
	Q_INVOKABLE QPoint getBottomRight() const;
	
	Q_INVOKABLE int getWidth() const;
	Q_INVOKABLE void setWidth(int width);
	
	Q_INVOKABLE int getHeight() const;
	Q_INVOKABLE void setHeight(int height);
	
	Q_INVOKABLE QString toString() const;
};

#endif // ENGINE_PROTOTYPES_RECT_PROTOTYPE_H
