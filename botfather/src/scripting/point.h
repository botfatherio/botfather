#ifndef POINT_H
#define POINT_H

#include <QObject>
#include <QScriptEngine>

class Point : public QObject
{
	Q_OBJECT
public:
	Point();
	explicit Point(int x, int y);
	
	static QScriptValue toScriptValue(QScriptEngine *engine, Point* const &in);
	static void fromScriptValue(const QScriptValue &object, Point* &out);
	
	Q_INVOKABLE int getX() const;
	Q_INVOKABLE int getY() const;
	Q_INVOKABLE void setX(int x);
	Q_INVOKABLE void setY(int y);
	
private:
	int m_x = 0;
	int m_y = 0;
};

Q_DECLARE_METATYPE(Point*)

#endif // POINT_H