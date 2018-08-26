#include "vision_api.h"
#include <QDebug>
#include <QFileInfo>
#include "../modules/vision/vision.h"

QScriptValue VisionAPI::findMaskedMatches(QImage image, QImage tpl, QImage mask, double threshold, int max_matches)
{
	MB_NOT_NULL(image, "image");
	MB_NOT_NULL(tpl, "tpl");
	MB_NOT_NULL(mask, "mask");
	MB_SMALLER(tpl, image, "tpl", "image");
	MB_SAME_SIZE(tpl, mask, "tpl", "mask");	

    cv::Mat ref_mat = Vision::qimageToBGRMat(image);
    cv::Mat tpl_mat = Vision::qimageToBGRMat(tpl);
    cv::Mat msk_mat = Vision::qimageToBGRMat(mask);
    
    QVector<Match> matches = Vision::findMaskedMatches(ref_mat, tpl_mat, msk_mat, threshold, max_matches);
	
	ref_mat.release();
	tpl_mat.release();
	msk_mat.release();
	
    return qScriptValueFromSequence(engine(), matches);
}

QScriptValue VisionAPI::findMaskedMatch(QImage image, QImage tpl, QImage mask, double threshold)
{
	MB_NOT_NULL(image, "image");
	MB_NOT_NULL(tpl, "tpl");
	MB_NOT_NULL(mask, "mask");
	MB_SMALLER(tpl, image, "tpl", "image");
	MB_SAME_SIZE(tpl, mask, "tpl", "mask");	
    
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
	MB_NOT_NULL(image, "image");
	MB_NOT_NULL(tpl, "tpl");
	MB_SMALLER(tpl, image, "tpl", "image");
    
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
	MB_NOT_NULL(image, "image");
	MB_NOT_NULL(tpl, "tpl");
	MB_SMALLER(tpl, image, "tpl", "image");
    
    cv::Mat image_mat = Vision::qimageToBGRMat(image);
    cv::Mat tpl_mat = Vision::qimageToBGRMat(tpl);
    
    // NOTE: Dont call findMaskedMatch here instead to save some checks, it requires a non empty mask.
    Match match = Vision::findMatch(image_mat, tpl_mat, threshold);
    
    image_mat.release();
    tpl_mat.release();
    
    return engine()->toScriptValue(match);
}

QScriptValue VisionAPI::findBlobs(QImage image, BlobTpl blob_tpl, int min_distance, int min_repeatability)
{
	MB_NOT_NULL(image, "image");
    
	blob_tpl.setMinBlobDistance(min_distance);
	blob_tpl.setMinRepeatability(min_repeatability);
	
    cv::Mat image_mat = Vision::qimageToBGRMat(image);
	QList<Match> matches = Vision::findBlobs(image_mat, blob_tpl.getBlobParams());
	
    image_mat.release();
	return qScriptValueFromSequence(engine(), matches);;
}

QScriptValue VisionAPI::markMatches(QImage image, QScriptValue matches, QColor color, int thickness)
{
    MB_ARRAY(matches, "matches");
	MB_NOT_NULL(image, "image");
    
    QList<Match> native_matches;
    qScriptValueToSequence(matches, native_matches);
    
	QImage result = Vision::markMatches(image, native_matches, color, thickness);
	return engine()->toScriptValue(result);
}

QScriptValue VisionAPI::markMatch(QImage image, Match match, QColor color, int thickness)
{
    MB_NOT_NULL(image, "image");
	MB_FOUND(match, "match");
    
	QImage result = Vision::markMatches(image, { match }, color, thickness);
    return engine()->toScriptValue(result);
}