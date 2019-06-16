#ifndef BFP_ENGINE_PROTOTYPES_POINT_PROTOTYPE_H
#define BFP_ENGINE_PROTOTYPES_POINT_PROTOTYPE_H
#define POINT_PROTOTYPE_DOCS "https://botfather.io/docs/apiref/point/"

#include "abstract_prototype.hpp"
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
	Q_INVOKABLE void setX(int x);
	
	// Returns the points y value
	// http://doc.qt.io/archives/qt-5.10/qpoint.html#y
	Q_INVOKABLE int getY() const;
	
	// Sets the points y value
	// http://doc.qt.io/archives/qt-5.10/qpoint.html#setY
	Q_INVOKABLE void setY(int y);
	
	// Returns a QPoint object that is the sum of this point and other_point; each component is added separately.
	// http://doc.qt.io/archives/qt-5.10/qpoint.html#operator-2b
	Q_INVOKABLE QPoint pointAdded(const QPoint &other_point) const;

	// Returns a QPoint object that is formed by subtracting other_point from this point; each component is subtracted separately.
	// http://doc.qt.io/archives/qt-5.10/qpoint.html#operator-
	Q_INVOKABLE QPoint pointSubtracted(const QPoint &other_point) const;

	// Returns the dot product of this point and another
	// http://doc.qt.io/archives/qt-5.10/qpoint.html#dotProduct
	Q_INVOKABLE int dotProduct(const QPoint &other_point) const;
	
	// Returns the distance to another point
	Q_INVOKABLE int distanceTo(const QPoint &other_point) const;
	
	Q_INVOKABLE QString toString() const;
};

#endif // BFP_ENGINE_PROTOTYPES_POINT_PROTOTYPE_H
