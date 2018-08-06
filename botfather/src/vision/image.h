#ifndef BFP__VISION__IMAGE_H
#define BFP__VISION__IMAGE_H

#include <QObject>
#include <QScriptEngine>
#include <QImage>

class Image : public QObject
{
Q_OBJECT
	
public:
	// Constructs an empty invalid image.
	Image();
	
	// Used to contruct a valid image (as long as umat is not empty).
	Image(QImage qimage);
	
	static QScriptValue toScriptValue(QScriptEngine *engine, Image* const &in);
	static void fromScriptValue(const QScriptValue &object, Image* &out);
	
	QImage getQImage() const;
	Q_INVOKABLE bool isValid();
	Q_INVOKABLE int getWidth();
	Q_INVOKABLE int getHeight();
	
private:
	QImage qimage;
};

Q_DECLARE_METATYPE(Image*)

#endif // BFP__VISION__IMAGE_H