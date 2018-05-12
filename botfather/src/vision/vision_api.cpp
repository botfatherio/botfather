#include "vision_api.h"
#include <QDebug>
#include <QFileInfo>
#include "vision.h"
#include "match.h"
#include "hsv_color_factory.h"
#include "blob_tpl_factory.h"
#include "../scripting/helper_api.h"
#include "../scripting/bot.h"

VisionAPI::VisionAPI(Bot* bot_p, QScriptEngine* engine_p)
	: QObject(bot_p), m_bot_p(bot_p), m_engine_p(engine_p)
{}

// static
void VisionAPI::enable(Bot* bot_p, QScriptEngine* engine_p)
{
	HSVColorFactory::enable(engine_p);
	BlobTplFactory::enable(engine_p);
	
	QScriptValue vision_obj = engine_p->newQObject(new VisionAPI(bot_p, engine_p));
	engine_p->globalObject().setProperty("Vision", vision_obj);
}

void VisionAPI::saveImage(Image* image, QString path)
{
	path = this->m_bot_p->normalisePath(path);
	Vision::saveImage(image->getUMat(), path);
}

QScriptValue VisionAPI::loadImage(QString path) {
	path = this->m_bot_p->normalisePath(path);
	if (this->m_bot_p->fileExists(path)){
		return m_engine_p->newQObject(new Image(Vision::loadImage(path)));
	}
	// Trow exception instead?
	return m_engine_p->newQObject(new Image());
}

QScriptValue VisionAPI::cropImage(Image* image, int x_offset, int y_offset, int width, int height)
{
	QRect region(x_offset, y_offset, width, height);
	cv::UMat cropped_image = Vision::cropImage(image->getUMat(), region);
	return m_engine_p->newQObject(new Image(cropped_image));
}

QScriptValue VisionAPI::grayImage(Image *image)
{
	cv::UMat gray_image = Vision::grayImage(image->getUMat());
	return m_engine_p->newQObject(new Image(gray_image));
}

QScriptValue VisionAPI::resizeImage(Image *image, int new_width, int new_height)
{
	cv::UMat resized_image = Vision::resizeImage(image->getUMat(), new_width, new_height);
	return m_engine_p->newQObject(new Image(resized_image));
}

QScriptValue VisionAPI::isolateColor(Image *image, HSVColor* min_hsv, HSVColor* max_hsv, bool keep_color)
{
	cv::UMat result_image = Vision::isolateColor(image->getUMat(), min_hsv->getScalar(), max_hsv->getScalar(), keep_color);
	return m_engine_p->newQObject(new Image(result_image));
}

bool VisionAPI::sameImages(Image* image_1, Image* image_2)
{
	return Vision::sameImages(image_1->getUMat(), image_2->getUMat());
}

QScriptValue VisionAPI::findMaskedMatches(Image *image, Image *tpl, Image *mask, double threshold, int max_matches)
{
	QVector<Match*> matches = Vision::findMaskedMatches(image->getUMat(), tpl->getUMat(), mask->getUMat(), threshold, max_matches);
	QScriptValue js_matches = m_engine_p->newArray();
	
	for (int i = 0; i < matches.size(); i++) {
		QScriptValue js_match = m_engine_p->newQObject(matches[i]);
		js_matches.setProperty(i, js_match);
	}
	
	return js_matches;
}

QScriptValue VisionAPI::findMaskedMatch(Image *image, Image *tpl, Image *mask, double threshold)
{
	Match* match = Vision::findMaskedMatch(image->getUMat(), tpl->getUMat(), mask->getUMat(), threshold);
	return m_engine_p->newQObject(match);
}


QScriptValue VisionAPI::findMatches(Image *image, Image *tpl, double threshold, int max_matches)
{
	return VisionAPI::findMaskedMatches(image, tpl, new Image(), threshold, max_matches);
}

QScriptValue VisionAPI::findMatch(Image *image, Image *tpl, double threshold)
{
	return VisionAPI::findMaskedMatch(image, tpl, new Image(), threshold);
}

QScriptValue VisionAPI::findBlobs(BlobTpl *blob_tpl, Image *image)
{
	QVector<cv::KeyPoint> keypoints = Vision::findBlobs(blob_tpl, image->getUMat());
	
	QScriptValue matches = m_engine_p->newArray();
	int number_of_matches = 0;
	
	// Turn detected cv::KeyPoints into js compatible matches.
	for (cv::KeyPoint kp : keypoints) {
		
		int left = kp.pt.x + (kp.size / 2);
		int top = kp.pt.y + (kp.size / 2);
		
		QScriptValue js_match = m_engine_p->newQObject(new Match(1, left, top, kp.size, kp.size));
		matches.setProperty(number_of_matches, js_match);
		number_of_matches++;
	}
	
	return matches;
}

QScriptValue VisionAPI::markMatches(Image *image, QScriptValue matches, int r, int g, int b, int thickness)
{
	if (!matches.isArray()) {
		return QScriptValue();
	}
	
	int length = matches.property("length").toNumber();
	
	QVector<Match*> native_matches;
	
	for (int i = 0; i < length; i++) {
		// TODO: Eventually move this convertion code somewhere else from where it can be reused.
		
		double score = matches.property(i).property("getScore").call().toNumber();
		int left = matches.property(i).property("getLeft").call().toNumber();
		int top = matches.property(i).property("getTop").call().toNumber();
		int width = matches.property(i).property("getWidth").call().toNumber();
		int height = matches.property(i).property("getHeight").call().toNumber();

		native_matches.append(new Match(score, left, top, width, height));
	}
	
	cv::UMat umat = Vision::markMatches(image->getUMat(), native_matches, cv::Scalar(b, g, r), thickness);
	return m_engine_p->newQObject(new Image(umat));
}

QScriptValue VisionAPI::markMatch(Image *image, Match *match, int r, int g, int b, int thickness)
{
	cv::UMat result_image = Vision::markMatch(image->getUMat(), match, cv::Scalar(b, g, r), thickness);
	return m_engine_p->newQObject(new Image(result_image));
}