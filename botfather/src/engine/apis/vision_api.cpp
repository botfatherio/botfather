#include "vision_api.h"
#include <QDebug>
#include <QFileInfo>
#include "../modules/vision/vision.h"

QScriptValue VisionAPI::findMaskedMatches(QImage image, QImage tpl, QImage mask, double threshold, int max_matches)
{
    if (image.isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty image.");
    }
    if (tpl.isNull()){
	    return engine()->currentContext()->throwError("Invalid or empty template.");
    }
    if (mask.isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty mask.");
    }
    if (image.height() <= tpl.height() || image.width() <= tpl.width()) {
	    return engine()->currentContext()->throwError("The template must be smaller than the image.");
    }
    if (tpl.height() != mask.height() || tpl.width() != mask.width()) {
	    return engine()->currentContext()->throwError("Mask and template must have the same size.");
    }
    
    cv::Mat ref_mat = Vision::qimageToBGRMat(image);
    cv::Mat tpl_mat = Vision::qimageToBGRMat(tpl);
    cv::Mat msk_mat = Vision::qimageToBGRMat(mask);
    
    QVector<Match> matches = Vision::findMaskedMatches(ref_mat, tpl_mat, msk_mat, threshold, max_matches);
    return qScriptValueFromSequence(engine(), matches);
}

QScriptValue VisionAPI::findMaskedMatch(QImage image, QImage tpl, QImage mask, double threshold)
{
    if (image.isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty image.");
    }
    if (tpl.isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty template.");
    }
    if (mask.isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty mask.");
    }
    if (image.height() <= tpl.height() || image.width() <= tpl.width()) {
	    return engine()->currentContext()->throwError("The template must be smaller than the image.");
    }
    if (tpl.height() != mask.height() || tpl.width() != mask.width()) {
	    return engine()->currentContext()->throwError("Mask and template must have the same size.");
    }
    
    cv::Mat image_mat = Vision::qimageToBGRMat(image);
    cv::Mat tpl_mat = Vision::qimageToBGRMat(tpl);
    cv::Mat mask_mat = Vision::qimageToBGRMat(mask);
    
    Match match = Vision::findMaskedMatch(image_mat, tpl_mat, mask_mat, threshold);
    
    image_mat.release();
    tpl_mat.release();
    mask_mat.release();
    
    return engine()->toScriptValue(match);
}

QScriptValue VisionAPI::findMatches(QImage image, QImage tpl, double threshold, int max_matches)
{
    if (image.isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty image.");
    }
    if (tpl.isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty template.");
    }
    if (image.height() <= tpl.height() || image.width() <= tpl.width()) {
	    return engine()->currentContext()->throwError("The template must be smaller than the image.");
    }
    
    cv::Mat image_mat = Vision::qimageToBGRMat(image);
    cv::Mat tpl_mat = Vision::qimageToBGRMat(tpl);
    
    // NOTE: Dont call findMaskedMatches here instead to save some checks, it requires a non empty mask.
    QVector<Match> matches = Vision::findMatches(image_mat, tpl_mat, threshold, max_matches);
    
    image_mat.release();
    tpl_mat.release();
    
    return qScriptValueFromSequence(engine(), matches);
}

QScriptValue VisionAPI::findMatch(QImage image, QImage tpl, double threshold)
{
    if (image.isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty image.");
    }
    if (tpl.isNull()) {
	    return engine()->currentContext()->throwError("Invalid or empty template.");
    }
    if (image.height() <= tpl.height() || image.width() <= tpl.width()) {
	    return engine()->currentContext()->throwError("The template must be smaller than the image.");
    }
    
    cv::Mat image_mat = Vision::qimageToBGRMat(image);
    cv::Mat tpl_mat = Vision::qimageToBGRMat(tpl);
    
    // NOTE: Dont call findMaskedMatch here instead to save some checks, it requires a non empty mask.
    Match match = Vision::findMatch(image_mat, tpl_mat, threshold);
    
    image_mat.release();
    tpl_mat.release();
    
    return engine()->toScriptValue(match);
}

/*
QScriptValue VisionAPI::findBlobs(BlobTpl blob_tpl, Image image)
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