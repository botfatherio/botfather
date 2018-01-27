#ifndef BFP__VISION__VISION_API_H
#define BFP__VISION__VISION_API_H

#include <opencv2/opencv.hpp>
#include <QObject>
#include <QJSEngine>
#include "image.h"
#include "blob_tpl.h"
#include "hsv_color.h"

class VisionAPI : public QObject
{
	Q_OBJECT
	
public:
	VisionAPI(QJSEngine* engine_p);
	static void enable(QJSEngine* engine_p);
	
	Q_INVOKABLE void saveImage(Image* image, QString path);
	Q_INVOKABLE QJSValue loadImage(QString path);
	Q_INVOKABLE QJSValue cropImage(Image* image, int x_offset, int y_offset, int width, int height);
	Q_INVOKABLE QJSValue grayImage(Image* image);
	Q_INVOKABLE QJSValue isolateColor(Image* image, HSVColor* min_hsv, HSVColor* max_hsv, bool keep_color = false);
	Q_INVOKABLE bool sameImages(Image* image_1, Image* image_2);
	Q_INVOKABLE QJSValue findMatches(Image* image, Image* tpl, double threshold = 0.8, int max_matches = 7);
	Q_INVOKABLE QJSValue findBlobs(BlobTpl* blob_tpl, Image* image );
	
private:
	QJSEngine* m_engine_p;
};

#endif // BFP__VISION__VISION_API_H