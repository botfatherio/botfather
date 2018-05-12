#ifndef BFP__VISION__IMAGE_H
#define BFP__VISION__IMAGE_H

#include <QObject>
#include <QScriptEngine>
#include <opencv2/opencv.hpp>

class Image : public QObject
{
Q_OBJECT
	
public:
	// Constructs an empty invalid image.
	Image();
	
	// Used to contruct a valid image (as long as umat is not empty).
	Image(cv::UMat umat);

	static QScriptValue toScriptValue(QScriptEngine *engine, Image* const &in);
	static void fromScriptValue(const QScriptValue &object, Image* &out);
	
	cv::UMat getUMat() const;
	
	Q_INVOKABLE bool isValid();
	Q_INVOKABLE int getWidth();
	Q_INVOKABLE int getHeight();
	
private:
	cv::UMat m_umat;
};

Q_DECLARE_METATYPE(Image*)

#endif // BFP__VISION__IMAGE_H