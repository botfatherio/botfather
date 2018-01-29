#include "vision_api.h"
#include <QDebug>
#include "vision.h"
#include "match.h"
#include "hsv_color_factory.h"
#include "blob_tpl_factory.h"
#include "../scripting/bot.h"

VisionAPI::VisionAPI(QJSEngine* engine_p)
	: m_engine_p(engine_p)
{}

// static
void VisionAPI::enable(QJSEngine* engine_p)
{
	HSVColorFactory::enable(engine_p);
	BlobTplFactory::enable(engine_p);
	
	QJSValue vision_obj = engine_p->newQObject(new VisionAPI(engine_p));
	engine_p->globalObject().setProperty("Vision", vision_obj);
}

void VisionAPI::saveImage(Image* image, QString path)
{
	Vision::saveImage(image->getUMat(), path);
}

QJSValue VisionAPI::loadImage(QString path) {
	if (Bot::fileExists(path)){
		return m_engine_p->newQObject(new Image(Vision::loadImage(path)));
	}
	return QJSValue();
}

QJSValue VisionAPI::cropImage(Image* image, int x_offset, int y_offset, int width, int height)
{
	QRect region(x_offset, y_offset, width, height);
	cv::UMat cropped_image = Vision::cropImage(image->getUMat(), region);
	return m_engine_p->newQObject(new Image(cropped_image));
}

QJSValue VisionAPI::grayImage(Image *image)
{
	cv::UMat gray_image = Vision::grayImage(image->getUMat());
	return m_engine_p->newQObject(new Image(gray_image));
}

QJSValue VisionAPI::isolateColor(Image *image, HSVColor* min_hsv, HSVColor* max_hsv, bool keep_color)
{
	cv::UMat result_image = Vision::isolateColor(image->getUMat(), min_hsv->getScalar(), max_hsv->getScalar(), keep_color);
	return m_engine_p->newQObject(new Image(result_image));
}

bool VisionAPI::sameImages(Image* image_1, Image* image_2)
{
	return Vision::sameImages(image_1->getUMat(), image_2->getUMat());
}

QJSValue VisionAPI::findMatches(Image *image, Image *tpl, double threshold, int max_matches)
{
	QVector<Match*> matches = Vision::findMatches(image->getUMat(), tpl->getUMat(), threshold, max_matches);
	QJSValue js_matches = m_engine_p->newArray();
	
	for (int i = 0; i < matches.size(); i++){
		// TODO: Thing about what happens here. Does JSEngine take over the ownage of the Match object?
		QJSValue js_match = m_engine_p->newQObject(matches[i]);
		js_matches.setProperty(i, js_match);
	}
	
	return js_matches;
}

QJSValue VisionAPI::findBlobs(BlobTpl *blob_tpl, Image *image)
{
	QVector<cv::KeyPoint> keypoints = Vision::findBlobs(blob_tpl, image->getUMat());
	
	QJSValue matches = m_engine_p->newArray();
	int number_of_matches = 0;
	
	// Turn detected cv::KeyPoints into js compatible matches.
	for (cv::KeyPoint kp : keypoints) {
		
		QRect match_rect;
		match_rect.setX(kp.pt.x);
		match_rect.setY(kp.pt.y);
		match_rect.setSize({ (int)kp.size, (int)kp.size });
		
		QJSValue js_match = m_engine_p->newQObject(new Match(1, match_rect));
		matches.setProperty(number_of_matches, js_match);
		number_of_matches++;
	}
	
	return matches;
}