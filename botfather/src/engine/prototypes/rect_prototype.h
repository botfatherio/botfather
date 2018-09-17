#ifndef BFP_ENGINE_PROTOTYPES_RECT_PROTOTYPE_H
#define BFP_ENGINE_PROTOTYPES_RECT_PROTOTYPE_H
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

	// Returns true if the rectangle is empty, otherwise returns false.
	// https://doc.qt.io/qt-5/qrect.html#isEmpty
	Q_INVOKABLE bool isEmpty() const;
	
	// Returns the y-coordinate of the rectangle's top edge. 
	// https://doc.qt.io/qt-5/qrect.html#top
	Q_INVOKABLE int getTop() const;
	
	// Returns x-the coordinate of the rectangle's left edge.
	// https://doc.qt.io/qt-5/qrect.html#left
	Q_INVOKABLE int getLeft() const;
	
	// Returns the x-coordinate of the rectangle's right edge.
	// https://doc.qt.io/qt-5/qrect.html#right
	Q_INVOKABLE int getRight() const;
	
	// Returns the coordinate of the rectangle's bottom edge.
	// https://doc.qt.io/qt-5/qrect.html#bottom
	Q_INVOKABLE int getBottom() const;
	
	// Returns the center point of the rectangle.
	// https://doc.qt.io/qt-5/qrect.html#center
	 Q_INVOKABLE QPoint getCenter() const;
	
	// Returns the position of the rectangle's top-left corner.
	// https://doc.qt.io/qt-5/qrect.html#topLeft
	Q_INVOKABLE QPoint getTopLeft() const;
	
	// Returns the position of the rectangle's top-right corner.
	// https://doc.qt.io/qt-5/qrect.html#topRight
	Q_INVOKABLE QPoint getTopRight() const;
	
	// Returns the position of the rectangle's bottom-left corner.
	// https://doc.qt.io/qt-5/qrect.html#bottomLeft
	Q_INVOKABLE QPoint getBottomLeft() const;
	
	// Returns the position of the rectangle's bottom-right corner.
	// https://doc.qt.io/qt-5/qrect.html#bottomRight
	Q_INVOKABLE QPoint getBottomRight() const;

	// Returns the width of the rectangle.
	// https://doc.qt.io/qt-5/qrect.html#width
	Q_INVOKABLE int getWidth() const;
	
	// Sets the width of the rectangle to the given width. The right edge is changed, but not the left one.
	// https://doc.qt.io/qt-5/qrect.html#setWidth
	Q_INVOKABLE void setWidth(int width);
	
	// Returns the height of the rectangle.
	// https://doc.qt.io/qt-5/qrect.html#height
	Q_INVOKABLE int getHeight() const;
	
	// Sets the height of the rectangle to the given height. The bottom edge is changed, but not the top one.
	// https://doc.qt.io/qt-5/qrect.html#setHeight
	Q_INVOKABLE void setHeight(int height);
	
	// Returns the size of the rectangle.
	// https://doc.qt.io/qt-5/qrect.html#size
	Q_INVOKABLE QSize getSize() const;
	
	// Moves the rectangle vertically, leaving the rectangle's top edge at the given y coordinate. The rectangle's size is unchanged.
	// https://doc.qt.io/qt-5/qrect.html#moveTop
	Q_INVOKABLE void moveTop(int top);
	
	// Moves the rectangle horizontally, leaving the rectangle's left edge at the given x coordinate. The rectangle's size is unchanged.
	// https://doc.qt.io/qt-5/qrect.html#moveLeft
	Q_INVOKABLE void moveLeft(int left);
	
	// Moves the rectangle horizontally, leaving the rectangle's right edge at the given x coordinate. The rectangle's size is unchanged.
	// https://doc.qt.io/qt-5/qrect.html#moveRight
	Q_INVOKABLE void moveRight(int right);
	
	// Moves the rectangle vertically, leaving the rectangle's bottom edge at the given y coordinate. The rectangle's size is unchanged.
	// https://doc.qt.io/qt-5/qrect.html#moveBottom
	Q_INVOKABLE void moveBottom(int bottom);

	// Returns a rectangle grown by the margins.
	// https://doc.qt.io/qt-5/qrect.html#marginsAdded
	Q_INVOKABLE QRect marginsAdded(const QMargins &margins) const;

	// Removes the margins from the rectangle, shrinking it.
	// https://doc.qt.io/qt-5/qrect.html#marginsRemoved
	Q_INVOKABLE QRect marginsSubtracted(const QMargins &margins) const;

	// Returns true if the given point is inside or on the edge of the rectangle, otherwise returns false.
	// If proper is true, this function only returns true if the given point is inside the rectangle (i.e., not on the edge).
	// https://doc.qt.io/qt-5/qrect.html#contains
	Q_INVOKABLE bool contains(const QPoint &point, bool proper = false) const;
	
	// Returns true if the given rectangle is inside this rectangle.
	// https://doc.qt.io/qt-5/qrect.html#contains-1
	Q_INVOKABLE bool contains(const QRect &other_rect, bool proper = false) const;
		
	// Returns true if this rectangle intersects with the given rectangle, otherwise returns false.
	// https://doc.qt.io/qt-5/qrect.html#intersects
	Q_INVOKABLE bool intersects(const QRect &other_rect) const;
	
	// Returns the bounding rectangle of this rectangle and the given rectangle.
	// https://doc.qt.io/qt-5/qrect.html#united
	Q_INVOKABLE QRect united(const QRect &other_rect) const;

	// Generates and returns a random point within the rectangle.
	Q_INVOKABLE QPoint randomPoint() const;
	
	Q_INVOKABLE QString toString() const;
};

#endif // BFP_ENGINE_PROTOTYPES_RECT_PROTOTYPE_H
