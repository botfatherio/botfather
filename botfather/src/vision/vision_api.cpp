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
	
	QScriptValue vision_obj = engine_p->newQObject(new VisionAPI(bot_p, engine_p), QScriptEngine::ScriptOwnership);
	engine_p->globalObject().setProperty("Vision", vision_obj);
}

void VisionAPI::saveImage(Image* image, QString path)
{
	if (!image || image->getQImage().isNull()) {
		m_engine_p->currentContext()->throwError("Invalid or empty image.");
		return;
	}
	if (path.isEmpty()) {
		m_engine_p->currentContext()->throwError("Empty path.");
		return;
	}
	path = this->m_bot_p->normalisePath(path);
	image->getQImage().save(path);
}

QScriptValue VisionAPI::loadImage(QString path) {
	path = this->m_bot_p->normalisePath(path);
	if (this->m_bot_p->fileExists(path)){
		QImage qimage;
		qimage.load(path);
		m_engine_p->reportAdditionalMemoryCost(static_cast<int>(qimage.sizeInBytes()));
		return m_engine_p->newQObject(new Image(qimage), QScriptEngine::ScriptOwnership);
	}
	return m_engine_p->currentContext()->throwError("Invalid path.");
}

QScriptValue VisionAPI::cropImage(Image* image, int x_offset, int y_offset, int width, int height)
{
	if (!image || image->getQImage().isNull()) {
		return m_engine_p->currentContext()->throwError("Invalid or empty image.");
	}
	if (x_offset < 0 || y_offset < 0) {
		return m_engine_p->currentContext()->throwError("Offset must not be negativ.");
	}
	if (width <= 0 || height <= 0) {
		return m_engine_p->currentContext()->throwError("Width and height must be at least 1.");
	}
	QRect region(x_offset, y_offset, width, height);
	QImage cropped = image->getQImage().copy(region);
	m_engine_p->reportAdditionalMemoryCost(static_cast<int>(cropped.sizeInBytes()));
	return m_engine_p->newQObject(new Image(cropped), QScriptEngine::ScriptOwnership);
}

QScriptValue VisionAPI::grayImage(Image *image)
{
	if (!image || image->getQImage().isNull()) {
		return m_engine_p->currentContext()->throwError("Invalid or empty image.");
	}
	QImage grayscale = image->getQImage().convertToFormat(QImage::Format_Grayscale8);
	m_engine_p->reportAdditionalMemoryCost(static_cast<int>(grayscale.sizeInBytes()));
	return m_engine_p->newQObject(new Image(grayscale), QScriptEngine::ScriptOwnership);
}

QScriptValue VisionAPI::resizeImage(Image *image, int new_width, int new_height)
{
	if (!image || image->getQImage().isNull()) {
		return m_engine_p->currentContext()->throwError("Invalid or empty image.");
	}
	if (new_width <= 0 || new_height <= 0) {
		return m_engine_p->currentContext()->throwError("Width and height must be at least 1.");
	}
	QImage scaled = image->getQImage().scaled(new_width, new_height, Qt::KeepAspectRatio);
	m_engine_p->reportAdditionalMemoryCost(static_cast<int>(scaled.sizeInBytes()));
	return m_engine_p->newQObject(new Image(scaled), QScriptEngine::ScriptOwnership);
}

QScriptValue VisionAPI::isolateColor(Image *image, HSVColor* min_hsv, HSVColor* max_hsv, bool keep_color)
{
	if (!image || image->getQImage().isNull()) {
		return m_engine_p->currentContext()->throwError("Invalid or empty image.");
	}
	if (!min_hsv || !max_hsv) {
		m_engine_p->currentContext()->throwError("Invalid HSV color.");
	}
	
	cv::Mat mat = Vision::qimageToBGRMat(image->getQImage());
	cv::Mat result_image = Vision::isolateColor(mat, min_hsv->getScalar(), max_hsv->getScalar(), keep_color);
	mat.release();
	
	QImage qimage = Vision::cvMatToQImage(result_image);
	result_image.release();
	
	m_engine_p->reportAdditionalMemoryCost(static_cast<int>(qimage.sizeInBytes()));
	return m_engine_p->newQObject(new Image(qimage), QScriptEngine::ScriptOwnership);
}

bool VisionAPI::sameImages(Image* image_1, Image* image_2)
{
	if (!image_1 || !image_2) {
		m_engine_p->currentContext()->throwError("Invalid image(s).");
		return false;
	}
	cv::Mat mat1 = Vision::qimageToBGRMat(image_1->getQImage());
	cv::Mat mat2 = Vision::qimageToBGRMat(image_2->getQImage());
	bool result = Vision::sameImages(mat1, mat2);
	mat1.release();
	mat2.release();
	return result;
}

QScriptValue VisionAPI::findMaskedMatches(Image *image, Image *tpl, Image *mask, double threshold, int max_matches)
{
	if (!image || image->getQImage().isNull()) {
		return m_engine_p->currentContext()->throwError("Invalid or empty image.");
	}
	if (!tpl || tpl->getQImage().isNull()){
		return m_engine_p->currentContext()->throwError("Invalid or empty template.");
	}
	if (!mask || mask->getQImage().isNull()) {
		return m_engine_p->currentContext()->throwError("Invalid or empty mask.");
	}
	if (image->getQImage().height() <= tpl->getQImage().height() || image->getQImage().width() <= tpl->getQImage().width()) {
		return m_engine_p->currentContext()->throwError("The template must be smaller than the image.");
	}
	if (tpl->getQImage().height() != mask->getQImage().height() || tpl->getQImage().width() != mask->getQImage().width()) {
		return m_engine_p->currentContext()->throwError("Mask and template must have the same size.");
	}
	
	cv::Mat ref_mat = Vision::qimageToBGRMat(image->getQImage());
	cv::Mat tpl_mat = Vision::qimageToBGRMat(tpl->getQImage());
	cv::Mat msk_mat = Vision::qimageToBGRMat(mask->getQImage());
	
	QVector<Match*> matches = Vision::findMaskedMatches(ref_mat, tpl_mat, msk_mat, threshold, max_matches);
	return qScriptValueFromSequence(m_engine_p, matches);
}

QScriptValue VisionAPI::findMaskedMatch(Image *image, Image *tpl, Image *mask, double threshold)
{
	if (!image || image->getQImage().isNull()) {
		return m_engine_p->currentContext()->throwError("Invalid or empty image.");
	}
	if (!tpl || tpl->getQImage().isNull()) {
		return m_engine_p->currentContext()->throwError("Invalid or empty template.");
	}
	if (!mask || mask->getQImage().isNull()) {
		return m_engine_p->currentContext()->throwError("Invalid or empty mask.");
	}
	if (image->getQImage().height() <= tpl->getQImage().height() || image->getQImage().width() <= tpl->getQImage().width()) {
		return m_engine_p->currentContext()->throwError("The template must be smaller than the image.");
	}
	if (tpl->getQImage().height() != mask->getQImage().height() || tpl->getQImage().width() != mask->getQImage().width()) {
		return m_engine_p->currentContext()->throwError("Mask and template must have the same size.");
	}
	
	cv::Mat image_mat = Vision::qimageToBGRMat(image->getQImage());
	cv::Mat tpl_mat = Vision::qimageToBGRMat(tpl->getQImage());
	cv::Mat mask_mat = Vision::qimageToBGRMat(mask->getQImage());
	
	Match* match = Vision::findMaskedMatch(image_mat, tpl_mat, mask_mat, threshold);
	
	image_mat.release();
	tpl_mat.release();
	mask_mat.release();
	
	return m_engine_p->newQObject(match, QScriptEngine::ScriptOwnership);
}

QScriptValue VisionAPI::findMatches(Image *image, Image *tpl, double threshold, int max_matches)
{
	if (!image || image->getQImage().isNull()) {
		return m_engine_p->currentContext()->throwError("Invalid or empty image.");
	}
	if (!tpl || tpl->getQImage().isNull()) {
		return m_engine_p->currentContext()->throwError("Invalid or empty template.");
	}
	if (image->getQImage().height() <= tpl->getQImage().height() || image->getQImage().width() <= tpl->getQImage().width()) {
		return m_engine_p->currentContext()->throwError("The template must be smaller than the image.");
	}
	
	cv::Mat image_mat = Vision::qimageToBGRMat(image->getQImage());
	cv::Mat tpl_mat = Vision::qimageToBGRMat(tpl->getQImage());
	
	// NOTE: Dont call findMaskedMatches here instead to save some checks, it requires a non empty mask.
	QVector<Match*> matches = Vision::findMatches(image_mat, tpl_mat, threshold, max_matches);
	
	image_mat.release();
	tpl_mat.release();
	
	return qScriptValueFromSequence(m_engine_p, matches);
}

QScriptValue VisionAPI::findMatch(Image *image, Image *tpl, double threshold)
{
	if (!image || image->getQImage().isNull()) {
		return m_engine_p->currentContext()->throwError("Invalid or empty image.");
	}
	if (!tpl || tpl->getQImage().isNull()) {
		return m_engine_p->currentContext()->throwError("Invalid or empty template.");
	}
	if (image->getQImage().height() <= tpl->getQImage().height() || image->getQImage().width() <= tpl->getQImage().width()) {
		return m_engine_p->currentContext()->throwError("The template must be smaller than the image.");
	}
	
	cv::Mat image_mat = Vision::qimageToBGRMat(image->getQImage());
	cv::Mat tpl_mat = Vision::qimageToBGRMat(tpl->getQImage());
	
	// NOTE: Dont call findMaskedMatch here instead to save some checks, it requires a non empty mask.
	Match *match = Vision::findMatch(image_mat, tpl_mat, threshold);
	
	image_mat.release();
	tpl_mat.release();
	
	return m_engine_p->newQObject(match, QScriptEngine::ScriptOwnership);
}

QScriptValue VisionAPI::findBlobs(BlobTpl *blob_tpl, Image *image)
{
	if (!image || image->getQImage().isNull()) {
		return m_engine_p->currentContext()->throwError("Invalid or empty image.");
	}
	if (!blob_tpl) {
		return m_engine_p->currentContext()->throwError("Invalid template.");
	}
	
	cv::Mat image_mat = Vision::qimageToBGRMat(image->getQImage());
	
	QVector<cv::KeyPoint> keypoints = Vision::findBlobs(blob_tpl, image_mat);
	
	image_mat.release();
	
	QScriptValue matches = m_engine_p->newArray();
	int number_of_matches = 0;
	
	// Turn detected cv::KeyPoints into js compatible matches. cv:KeyPoint can't be converted
	// using qScriptValueFromSequence, unless we make them QObjects which the script engine
	// can interpret.
	for (cv::KeyPoint kp : keypoints) {
		
		int left = static_cast<int>(kp.pt.x + (kp.size / 2));
		int top = static_cast<int>(kp.pt.y + (kp.size / 2));
		
		Match *match = new Match(1, left, top, static_cast<int>(kp.size), static_cast<int>(kp.size));
		QScriptValue js_match = m_engine_p->newQObject(match, QScriptEngine::ScriptOwnership);
		matches.setProperty(static_cast<quint32>(number_of_matches), js_match);
		number_of_matches++;
	}
	
	return matches;
}

QScriptValue VisionAPI::markMatches(Image *image, QScriptValue matches, int r, int g, int b, int thickness)
{
	if (!image || image->getQImage().isNull()) {
		return m_engine_p->currentContext()->throwError("Invalid or empty image.");
	}
	if (!matches.isArray()) {
		return m_engine_p->currentContext()->throwError("Matches must be an array.");
	}
	
	QVector<Match*> native_matches;
	qScriptValueToSequence(matches, native_matches);
	
	
	cv::Mat image_mat = Vision::qimageToBGRMat(image->getQImage());
	
	cv::Mat mat = Vision::markMatches(image_mat, native_matches, cv::Scalar(b, g, r), thickness);
	QImage qimage = Vision::cvMatToQImage(mat);
	
	mat.release();
	image_mat.release();
	
	m_engine_p->reportAdditionalMemoryCost(static_cast<int>(qimage.sizeInBytes()));
	return m_engine_p->newQObject(new Image(qimage), QScriptEngine::ScriptOwnership);
}

QScriptValue VisionAPI::markMatch(Image *image, Match *match, int r, int g, int b, int thickness)
{
	if (!image || image->getQImage().isNull()) {
		return m_engine_p->currentContext()->throwError("Invalid or empty image.");
	}
	if (!match) {
		return m_engine_p->currentContext()->throwError("Invalid match.");
	}
	
	cv::Mat image_mat = Vision::qimageToBGRMat(image->getQImage());
	
	cv::Mat result_image = Vision::markMatch(image_mat, match, cv::Scalar(b, g, r), thickness);
	QImage qimage = Vision::cvMatToQImage(result_image);
	
	image_mat.release();
	
	m_engine_p->reportAdditionalMemoryCost(static_cast<int>(qimage.sizeInBytes()));
	return m_engine_p->newQObject(new Image(qimage), QScriptEngine::ScriptOwnership);
}