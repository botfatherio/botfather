#ifndef BFP_ENGINE_PROTOTYPES_COLOR_PROTOTYPE_H
#define BFP_ENGINE_PROTOTYPES_COLOR_PROTOTYPE_H
#define COLOR_PROTOTYPE_DOCS "https://botfather.io/docs/apiref/color/"

#include "abstract_prototype.h"
#include <QColor>

Q_DECLARE_METATYPE(QColor)
Q_DECLARE_METATYPE(QColor*)

class ColorPrototype : public AbstractPrototype
{
	Q_OBJECT
	
public:
	static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
	using AbstractPrototype::AbstractPrototype;
	
	// Returns true if the color is valid; otherwise returns false.
	// http://doc.qt.io/archives/qt-5.10/qcolor.html#isValid
	Q_INVOKABLE bool isValid() const;
	
	// Returns the red color component of this color.
	// http://doc.qt.io/archives/qt-5.10/qcolor.html#red
	Q_INVOKABLE int getRed() const;
	
	// Sets the red color component of this color to red. Integer components are specified in the range 0-255.
	// http://doc.qt.io/archives/qt-5.10/qcolor.html#setRed
	Q_INVOKABLE void setRed(int red);
	
	// Returns the green color component of this color.
	// http://doc.qt.io/archives/qt-5.10/qcolor.html#green
	Q_INVOKABLE int getGreen() const;
	
	// Sets the green color component of this color to green. Integer components are specified in the range 0-255.
	// http://doc.qt.io/archives/qt-5.10/qcolor.html#setGreen
	Q_INVOKABLE void setGreen(int green);
	
	// Returns the blue color component of this color.
	// http://doc.qt.io/archives/qt-5.10/qcolor.html#blue
	Q_INVOKABLE int getBlue() const;
	
	// Sets the blue color component of this color to blue. Integer components are specified in the range 0-255.
	// http://doc.qt.io/archives/qt-5.10/qcolor.html#setBlue
	Q_INVOKABLE void setBlue(int blue);
	
	// Sets a HSV color value; h is the hue, s is the saturation, v is the value and a is the alpha component of the HSV color.
	// The saturation, value and alpha-channel values must be in the range 0-255, and the hue value must be greater than -1.
	// http://doc.qt.io/archives/qt-5.10/qcolor.html#setHsv
	Q_INVOKABLE void setHsv(int h, int s, int v);
	
	// Returns the hue color component of this color.
	// http://doc.qt.io/archives/qt-5.10/qcolor.html#hsvHue
	Q_INVOKABLE int hue() const;
	
	// Returns the saturation color component of this color.
	// http://doc.qt.io/archives/qt-5.10/qcolor.html#hsvSaturation
	Q_INVOKABLE int saturation() const;
	
	// Returns the value color component of this color.
	// http://doc.qt.io/archives/qt-5.10/qcolor.html#value
	Q_INVOKABLE int value() const;

	Q_INVOKABLE QString toString() const;
};

#endif // BFP_ENGINE_PROTOTYPES_COLOR_PROTOTYPE_H
