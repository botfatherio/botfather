#ifndef BFP__VISION__VISION_API_H
#define BFP__VISION__VISION_API_H

#include <opencv2/opencv.hpp>
#include <QObject>
#include <QScriptEngine>
#include "match.h"

/*
	NOTE: QColor HSV is H: 0 - 359, S: 0 - 255, V: 0 - 255
	NOTE: OpenCV HSV is H: 0 - 180, S: 0 - 255, V: 0 - 255
*/

class Bot;

class VisionAPI : public QObject
{
	Q_OBJECT
	
public:
	VisionAPI(Bot* bot_p, QScriptEngine* engine_p);
	static void enable(Bot* bot_p, QScriptEngine* engine_p);
	
	Q_INVOKABLE void saveImage(QImage* image, QString path);
	Q_INVOKABLE QScriptValue loadImage(QString path);
	Q_INVOKABLE QScriptValue cropImage(QImage* image, int x_offset, int y_offset, int width, int height);
	Q_INVOKABLE QScriptValue grayImage(QImage* image);
	Q_INVOKABLE QScriptValue resizeImage(QImage* image, int new_width, int new_height);
	Q_INVOKABLE QScriptValue isolateColor(QImage* image, QColor *min_color, QColor *max_color, bool keep_color = false);
	Q_INVOKABLE bool sameImages(QImage* image_1, QImage* image_2);
	
	Q_INVOKABLE QScriptValue findMaskedMatches(QImage* image, QImage* tpl, QImage* mask, double threshold = 0.8, int max_matches = -1);
	Q_INVOKABLE QScriptValue findMaskedMatch(QImage* image, QImage* tpl, QImage* mask, double threshold = 0.8);
	
	Q_INVOKABLE QScriptValue findMatches(QImage* image, QImage* tpl, double threshold = 0.8, int max_matches = -1);
	Q_INVOKABLE QScriptValue findMatch(QImage* image, QImage* tpl, double threshold = 0.8);
	
	//Q_INVOKABLE QScriptValue findBlobs(BlobTpl* blob_tpl, Image* image );
	Q_INVOKABLE QScriptValue markMatches(QImage* image, QScriptValue matches, int r = 255, int g = 0, int b = 255, int thickness = 2);
	Q_INVOKABLE QScriptValue markMatch(QImage* image, Match* match, int r = 255, int g = 0, int b = 255, int thickness = 2);
	
private:
	Bot* m_bot_p;
	QScriptEngine* m_engine_p;
};

#endif // BFP__VISION__VISION_API_H