#ifndef BFP__VISION__HSV_COLOR_H
#define BFP__VISION__HSV_COLOR_H

#include <QObject>
#include <QScriptEngine>
#include <opencv2/opencv.hpp>

class HSVColor : public QObject
{
	Q_OBJECT

public:
	explicit HSVColor();
	explicit HSVColor(int h, int s, int v);
	
	static QScriptValue toScriptValue(QScriptEngine *engine, HSVColor* const &in);
	static void fromScriptValue(const QScriptValue &object, HSVColor* &out);
	
	Q_INVOKABLE void setH(int h);
	Q_INVOKABLE int getH() const;
	
	Q_INVOKABLE void setS(int s);
	Q_INVOKABLE int getS() const;
	
	Q_INVOKABLE void setV(int v);
	Q_INVOKABLE int getV() const;
	
	// OpenCV represents HSV as H: 0-180, S: 0-255, V: 0-255, but GIMP and Photoshop
	// represent it as H: 0-360°, S: 0-100%, V: 0-100%. The laters one is what we
	// expect as parameter thus we have to convert it to the OpenCV format.
	cv::Scalar getScalar();
	
private:
	int m_h = 0; // 0-360° hue
	int m_s = 0; // 0-100% saturation
	int m_v = 0; // 0-100% value
};

Q_DECLARE_METATYPE(HSVColor*)

#endif // BFP__VISION__HSV_COLOR_H