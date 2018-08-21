#ifndef BFP_ENGINE_APIS_VISION_VISION_API_H
#define BFP_ENGINE_APIS_VISION_VISION_API_H

#include <QImage>
#include "abstract_api.h"
#include "../types/match.h"

class VisionAPI : public AbstractAPI
{
	Q_OBJECT
	
public:
	using AbstractAPI::AbstractAPI;
	
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
	Q_INVOKABLE QScriptValue markMatches(QImage image, QScriptValue matches, QColor color, int thickness = 2);
	Q_INVOKABLE QScriptValue markMatch(QImage image, Match match, QColor color, int thickness = 2);
};

#endif // BFP_ENGINE_APIS_VISION_VISION_API_H