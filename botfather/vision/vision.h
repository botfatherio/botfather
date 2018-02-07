#ifndef BFP__VISION__VISION_H
#define BFP__VISION__VISION_H

#include <opencv2/opencv.hpp>
#include <QString>
#include <QRect>
#include <QVector>

class BlobTpl;
class Match;

class Vision
{
public:
	static void saveImage(cv::UMat image, QString path);
	static cv::UMat loadImage(QString path);
	static cv::UMat cropImage(cv::UMat image, QRect region);
	static cv::UMat grayImage(cv::UMat image);
	static cv::UMat isolateColor(cv::UMat image, cv::Scalar min_hsv, cv::Scalar max_hsv, bool keep_color);
	static bool sameImages(cv::UMat image_1, cv::UMat image_2);
	static QVector<Match*> findMatches(cv::UMat image, cv::UMat tpl, double threshold, int max_matches);
	static QVector<cv::KeyPoint> findBlobs(BlobTpl *blob_tpl, cv::UMat image);
};

#endif // BFP__VISION__VISION_H