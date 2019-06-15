#ifndef BFP_ENGINE_MODULES_VISION_VISION_H
#define BFP_ENGINE_MODULES_VISION_VISION_H

#include <opencv2/opencv.hpp>
#include <QString>
#include <QRect>
#include <QVector>
#include <QImage>

class Match;
class BlobTpl;

namespace Vision
{
	cv::Mat isolateColor(const cv::Mat &image, const cv::Scalar &min_hsv, const cv::Scalar &max_hsv, bool keep_color);
	
	int countDifferentPixels(const cv::Mat &image_1, const cv::Mat &image_2);
	
	// https://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/histogram_calculation/histogram_calculation.html
	// https://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/histogram_comparison/histogram_comparison.html
	double histogramSimilarity(const cv::Mat &image_1, const cv::Mat &image_2);
	
	QVector<Match> findMaskedMatches(const cv::Mat &image, const cv::Mat &tpl, const cv::Mat &mask, double threshold, int max_matches);
	Match findMaskedMatch(const cv::Mat &image, const cv::Mat &tpl, const cv::Mat &mask, double threshold);
	
	QVector<Match> findMatches(const cv::Mat &image, const cv::Mat &tpl, double threshold, int max_matches = 7);
	Match findMatch(const cv::Mat &image, const cv::Mat &tpl, double threshold);
	
	// https://docs.opencv.org/2.4/modules/features2d/doc/common_interfaces_of_feature_detectors.html#simpleblobdetector
	// https://docs.opencv.org/trunk/d8/da7/structcv_1_1SimpleBlobDetector_1_1Params.html
	QVector<Match> findBlobs(const cv::Mat &image, const cv::SimpleBlobDetector::Params &blob_params);
	
	QImage markMatches(const QImage &image, const QVector<Match> &matches, const QColor &color, int thickness = 2);
	
	// Turns a QImage into a 3 Channel BGR cv::UMat
	cv::Mat qimageToBGRMat(const QImage &qimage);
								  
	QImage cvMatToQImage(const cv::Mat &mat);
	
	cv::Scalar qColorToHsvScalar(const QColor &qcolor);
}

#endif // BFP_ENGINE_MODULES_VISION_VISION_H
