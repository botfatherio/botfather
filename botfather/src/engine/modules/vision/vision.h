#ifndef BFP_ENGINE_MODULES_VISION_VISION_H
#define BFP_ENGINE_MODULES_VISION_VISION_H

#include <opencv2/opencv.hpp>
#include <QString>
#include <QRect>
#include <QVector>
#include <QImage>

class Match;

class Vision
{
public:
	static cv::Mat isolateColor(cv::Mat image, cv::Scalar min_hsv, cv::Scalar max_hsv, bool keep_color);
	
	static int countDifferentPixels(cv::Mat image_1, cv::Mat image_2);
	
	// https://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/histogram_calculation/histogram_calculation.html
	// https://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/histogram_comparison/histogram_comparison.html
	static double histogramSimilarity(cv::Mat image_1, cv::Mat image_2);
	
	static QVector<Match> findMaskedMatches(cv::Mat image, cv::Mat tpl, cv::Mat mask, double threshold, int max_matches);
	static Match findMaskedMatch(cv::Mat image, cv::Mat tpl, cv::Mat mask, double threshold);
	
	static QVector<Match> findMatches(cv::Mat image, cv::Mat tpl, double threshold, int max_matches = 7);
	static Match findMatch(cv::Mat image, cv::Mat tpl, double threshold);
	
	//static QVector<cv::KeyPoint> findBlobs(BlobTpl *blob_tpl, cv::Mat image);
	static cv::Mat markMatches(cv::Mat image, QVector<Match> matches, cv::Scalar color = cv::Scalar(0, 0, 255), int thickness = 2);
	static cv::Mat markMatch(cv::Mat image, Match match, cv::Scalar color = cv::Scalar(0, 0, 255), int thickness = 2);
	
	// Turns a QImage into a 3 Channel BGR cv::UMat
	static cv::Mat qimageToBGRMat(const QImage &qimage);
								  
	static QImage cvMatToQImage(const cv::Mat &mat);
};

#endif // BFP_ENGINE_MODULES_VISION_VISION_H