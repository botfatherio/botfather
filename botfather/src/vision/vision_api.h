#ifndef BFP__VISION__VISION_API_H
#define BFP__VISION__VISION_API_H

#include <opencv2/opencv.hpp>
#include <QObject>
#include <QScriptEngine>
#include "image.h"
#include "blob_tpl.h"
#include "hsv_color.h"
#include "match.h"

class Bot;

class VisionAPI : public QObject
{
	Q_OBJECT
	
public:
	VisionAPI(Bot* bot_p, QScriptEngine* engine_p);
	static void enable(Bot* bot_p, QScriptEngine* engine_p);
	
	Q_INVOKABLE void saveImage(Image* image, QString path);
	Q_INVOKABLE QScriptValue loadImage(QString path);
	Q_INVOKABLE QScriptValue cropImage(Image* image, int x_offset, int y_offset, int width, int height);
	Q_INVOKABLE QScriptValue grayImage(Image* image);
	Q_INVOKABLE QScriptValue resizeImage(Image* image, int new_width, int new_height);
	Q_INVOKABLE QScriptValue isolateColor(Image* image, HSVColor* min_hsv, HSVColor* max_hsv, bool keep_color = false);
	Q_INVOKABLE bool sameImages(Image* image_1, Image* image_2);
	
	Q_INVOKABLE QScriptValue findMaskedMatches(Image* image, Image* tpl, Image* mask, double threshold, int max_matches);
	Q_INVOKABLE QScriptValue findMaskedMatch(Image* image, Image* tpl, Image* mask, double threshold);
	
	Q_INVOKABLE QScriptValue findMatches(Image* image, Image* tpl, double threshold = 0.8, int max_matches = 7);
	Q_INVOKABLE QScriptValue findMatch(Image* image, Image* tpl, double threshold = 0.8);
	
	Q_INVOKABLE QScriptValue findBlobs(BlobTpl* blob_tpl, Image* image );
	Q_INVOKABLE QScriptValue markMatches(Image* image, QScriptValue matches, int r = 255, int g = 0, int b = 255, int thickness = 2);
	Q_INVOKABLE QScriptValue markMatch(Image* image, Match* match, int r = 255, int g = 0, int b = 255, int thickness = 2);
	
private:
	Bot* m_bot_p;
	QScriptEngine* m_engine_p;
};

#endif // BFP__VISION__VISION_API_H