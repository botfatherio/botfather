#ifndef BFP_ENGINE_PROTOTYPES_POINT_PROTOTYPE_H
#define BFP_ENGINE_PROTOTYPES_POINT_PROTOTYPE_H
#define POINT_PROTOTYPE_DOCS "https://botfather.io/docs/apiref/point/"

#include "abstract_prototype.h"
#include <QPoint>

Q_DECLARE_METATYPE(QPoint)
Q_DECLARE_METATYPE(QPoint*)

class PointPrototype : public AbstractPrototype
{
	Q_OBJECT
	
public:
	static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
	using AbstractPrototype::AbstractPrototype;

	// Returns the points x value
	// http://doc.qt.io/archives/qt-5.10/qpoint.html#x
	Q_INVOKABLE int getX() const;
	
	// Sets the points x value
	// http://doc.qt.io/archives/qt-5.10/qpoint.html#setX
	Q_INVOKABLE void setX(int width);
	
	// Returns the points y value
	// http://doc.qt.io/archives/qt-5.10/qpoint.html#y
	Q_INVOKABLE int getY() const;
	
	// Sets the points y value
	// http://doc.qt.io/archives/qt-5.10/qpoint.html#setY
	Q_INVOKABLE void setY(int height);
	
	// Returns the dot product of this point and another
	// http://doc.qt.io/archives/qt-5.10/qpoint.html#dotProduct
	Q_INVOKABLE int dotProduct(QPoint other_point) const;
	
	// Returns the sum of the absolute values of x() and y(), traditionally known as the "Manhattan length" of the vector from the origin to the point
	// http://doc.qt.io/archives/qt-5.10/qpoint.html#manhattanLength
	Q_INVOKABLE int manhattanLength() const;
	
	Q_INVOKABLE QString toString() const;
};

#endif // BFP_ENGINE_PROTOTYPES_POINT_PROTOTYPE_H