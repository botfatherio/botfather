#include "vision_api.h"
#include <QDebug>
#include <QFileInfo>
#include "../modules/vision/vision.h"

void VisionAPI::saveImage(QImage* image, QString path)
{
    if (!image || image->isNull()) {
	    engine()->currentContext()->throwError("Invalid or empty image.");
	    return;
    }
    if (path.isEmpty()) {
	    engine()->currentContext()->throwError("Empty path.");
	    return;
    }
    path = bot()->normalisePath(path);
    image->save(path);
}

QScriptValue VisionAPI::loadImage(QString path)
{
    path = bot()->normalisePath(path);
    if (bot()->scriptFileExists(path)){
	    QImage qimage;
	    qimage.load(path);
	    qimage = qimage.convertToFormat(QImage::Format_RGB888);
	    
	    //engine()->reportAdditionalMemoryCost(static_cast<int>(ImageSizeInBytes(qimage)));
	    return engine()->toScriptValue(qimage);
    }
    return engine()->currentContext()->throwError("Invalid path.");
}

QScriptValue VisionAPI::cropImage(QImage* image, int x_offset, int y_offset, int width, int height)
{
    if (!image || image->isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty image.");
    }
    if (x_offset < 0 || y_offset < 0) {
	    return engine()->currentContext()->throwError("Offset must not be negativ.");
    }
    if (width <= 0 || height <= 0) {
	    return engine()->currentContext()->throwError("Width and height must be at least 1.");
    }
    QRect region(x_offset, y_offset, width, height);
    QImage cropped = image->copy(region);
    //engine()->reportAdditionalMemoryCost(static_cast<int>(ImageSizeInBytes(cropped)));
    return engine()->toScriptValue(cropped);
}

QScriptValue VisionAPI::grayImage(QImage *image)
{
    if (!image || image->isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty image.");
    }
    QImage grayscale = image->convertToFormat(QImage::Format_Grayscale8);
    //engine()->reportAdditionalMemoryCost(static_cast<int>(ImageSizeInBytes(grayscale)));
    return engine()->toScriptValue(grayscale);
}

QScriptValue VisionAPI::resizeImage(QImage *image, int new_width, int new_height)
{
    if (!image || image->isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty image.");
    }
    if (new_width <= 0 || new_height <= 0) {
	    return engine()->currentContext()->throwError("Width and height must be at least 1.");
    }
    QImage scaled = image->scaled(new_width, new_height, Qt::KeepAspectRatio);
    //engine()->reportAdditionalMemoryCost(static_cast<int>(ImageSizeInBytes(scaled)));
    return engine()->toScriptValue(scaled);
}

QScriptValue VisionAPI::isolateColor(QImage *image, QColor* min_color, QColor* max_color, bool keep_color)
{
    if (!image || image->isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty image.");
    }
    if (!min_color || !max_color) {
	    engine()->currentContext()->throwError("Invalid Color.");
    }
    
    cv::Scalar min_hsv(min_color->hsvHue() / 2, min_color->hsvSaturation(), min_color->value());
    cv::Scalar max_hsv(max_color->hsvHue() / 2, max_color->hsvSaturation(), max_color->value());
    
    cv::Mat mat = Vision::qimageToBGRMat(*image); // TODO: check whether this works or leaks mem
    cv::Mat result_image = Vision::isolateColor(mat, min_hsv, max_hsv, keep_color);
    mat.release();
    
    QImage qimage = Vision::cvMatToQImage(result_image);
    result_image.release();
    
    //engine()->reportAdditionalMemoryCost(static_cast<int>(ImageSizeInBytes(qimage)));
    return engine()->toScriptValue(qimage);
}

bool VisionAPI::sameImages(QImage* image_1, QImage* image_2)
{
    if (!image_1 || !image_2) {
	    engine()->currentContext()->throwError("Invalid image(s).");
	    return false;
    }
    cv::Mat mat1 = Vision::qimageToBGRMat(*image_1); // TODO: check whether this works or leaks mem
    cv::Mat mat2 = Vision::qimageToBGRMat(*image_2);
    bool result = Vision::sameImages(mat1, mat2);
    mat1.release();
    mat2.release();
    return result;
}

QScriptValue VisionAPI::findMaskedMatches(QImage *image, QImage *tpl, QImage *mask, double threshold, int max_matches)
{
    if (!image || image->isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty image.");
    }
    if (!tpl || tpl->isNull()){
	    return engine()->currentContext()->throwError("Invalid or empty template.");
    }
    if (!mask || mask->isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty mask.");
    }
    if (image->height() <= tpl->height() || image->width() <= tpl->width()) {
	    return engine()->currentContext()->throwError("The template must be smaller than the image.");
    }
    if (tpl->height() != mask->height() || tpl->width() != mask->width()) {
	    return engine()->currentContext()->throwError("Mask and template must have the same size.");
    }
    
    cv::Mat ref_mat = Vision::qimageToBGRMat(*image); // TODO: check whether this works or leaks mem
    cv::Mat tpl_mat = Vision::qimageToBGRMat(*tpl);
    cv::Mat msk_mat = Vision::qimageToBGRMat(*mask);
    
    QVector<Match> matches = Vision::findMaskedMatches(ref_mat, tpl_mat, msk_mat, threshold, max_matches);
    return qScriptValueFromSequence(engine(), matches);
}

QScriptValue VisionAPI::findMaskedMatch(QImage *image, QImage *tpl, QImage *mask, double threshold)
{
    if (!image || image->isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty image.");
    }
    if (!tpl || tpl->isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty template.");
    }
    if (!mask || mask->isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty mask.");
    }
    if (image->height() <= tpl->height() || image->width() <= tpl->width()) {
	    return engine()->currentContext()->throwError("The template must be smaller than the image.");
    }
    if (tpl->height() != mask->height() || tpl->width() != mask->width()) {
	    return engine()->currentContext()->throwError("Mask and template must have the same size.");
    }
    
    cv::Mat image_mat = Vision::qimageToBGRMat(*image); // TODO: check whether this works or leaks mem
    cv::Mat tpl_mat = Vision::qimageToBGRMat(*tpl);
    cv::Mat mask_mat = Vision::qimageToBGRMat(*mask);
    
    Match match = Vision::findMaskedMatch(image_mat, tpl_mat, mask_mat, threshold);
    
    image_mat.release();
    tpl_mat.release();
    mask_mat.release();
    
    return engine()->toScriptValue(match);
}

QScriptValue VisionAPI::findMatches(QImage *image, QImage *tpl, double threshold, int max_matches)
{
    if (!image || image->isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty image.");
    }
    if (!tpl || tpl->isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty template.");
    }
    if (image->height() <= tpl->height() || image->width() <= tpl->width()) {
	    return engine()->currentContext()->throwError("The template must be smaller than the image.");
    }
    
    cv::Mat image_mat = Vision::qimageToBGRMat(*image); // TODO: check whether this works or leaks mem
    cv::Mat tpl_mat = Vision::qimageToBGRMat(*tpl);
    
    // NOTE: Dont call findMaskedMatches here instead to save some checks, it requires a non empty mask.
    QVector<Match> matches = Vision::findMatches(image_mat, tpl_mat, threshold, max_matches);
    
    image_mat.release();
    tpl_mat.release();
    
    return qScriptValueFromSequence(engine(), matches);
}

QScriptValue VisionAPI::findMatch(QImage *image, QImage *tpl, double threshold)
{
    if (!image || image->isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty image.");
    }
    if (!tpl || tpl->isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty template.");
    }
    if (image->height() <= tpl->height() || image->width() <= tpl->width()) {
	    return engine()->currentContext()->throwError("The template must be smaller than the image.");
    }
    
    cv::Mat image_mat = Vision::qimageToBGRMat(*image); // TODO: check whether this works or leaks mem
    cv::Mat tpl_mat = Vision::qimageToBGRMat(*tpl);
    
    // NOTE: Dont call findMaskedMatch here instead to save some checks, it requires a non empty mask.
    Match match = Vision::findMatch(image_mat, tpl_mat, threshold);
    
    image_mat.release();
    tpl_mat.release();
    
    return engine()->toScriptValue(match);
}

/*
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
*/

QScriptValue VisionAPI::markMatches(QImage image, QScriptValue matches, QColor color, int thickness)
{
    if (image.isNull()) {
	    return engine()->currentContext()->throwError("The image must not be null.");
    }
    if (!matches.isArray()) {
	    return engine()->currentContext()->throwError("Matches must be an array.");
    }
    
    QVector<Match> native_matches;
    qScriptValueToSequence(matches, native_matches);
    
    cv::Mat image_mat = Vision::qimageToBGRMat(image);
    cv::Scalar cv_color(color.blue(), color.green(), color.red());
    cv::Mat mat = Vision::markMatches(image_mat, native_matches, cv_color, thickness);
    
    QImage qimage = Vision::cvMatToQImage(mat);
    //engine()->reportAdditionalMemoryCost(static_cast<int>(ImageSizeInBytes(qimage)));
    return engine()->toScriptValue(qimage);
}

QScriptValue VisionAPI::markMatch(QImage image, Match match, QColor color, int thickness)
{
    if (image.isNull()) {
	    return engine()->currentContext()->throwError(QScriptContext::TypeError, "The image must not be null.");
    }
    if (!match.found()) {
	    return engine()->currentContext()->throwError(QScriptContext::TypeError, "The match must be found.");
    }
    
    cv::Mat image_mat = Vision::qimageToBGRMat(image);
    cv::Scalar cv_color(color.blue(), color.green(), color.red());
    cv::Mat result_image = Vision::markMatch(image_mat, match, cv_color, thickness);
    
    QImage qimage = Vision::cvMatToQImage(result_image);
    //engine()->reportAdditionalMemoryCost(static_cast<int>(ImageSizeInBytes(qimage)));
    return engine()->toScriptValue(qimage);
}