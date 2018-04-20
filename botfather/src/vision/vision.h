#ifndef BFP__VISION__VISION_H
#define BFP__VISION__VISION_H

#include <opencv2/opencv.hpp>
#include <QString>
#include <QRect>
#include <QVector>
#include <QImage>

class BlobTpl;
class Match;

class Vision
{
public:
	static void saveImage(cv::UMat image, QString path);
	static cv::UMat loadImage(QString path);
	static cv::UMat cropImage(cv::UMat image, QRect region);
	static cv::UMat grayImage(cv::UMat image);
	static cv::UMat resizeImage(cv::UMat image, int new_width, int new_height);
	static cv::UMat isolateColor(cv::UMat image, cv::Scalar min_hsv, cv::Scalar max_hsv, bool keep_color);
	static bool sameImages(cv::UMat image_1, cv::UMat image_2);
	
	static QVector<Match*> findMaskedMatches(cv::UMat image, cv::UMat tpl, cv::UMat mask, double threshold, int max_matches);
	static Match* findMaskedMatch(cv::UMat image, cv::UMat tpl, cv::UMat mask, double threshold);
	
	static QVector<Match*> findMatches(cv::UMat image, cv::UMat tpl, double threshold, int max_matches = 7);
	static Match* findMatch(cv::UMat image, cv::UMat tpl, double threshold);
	
	static QVector<cv::KeyPoint> findBlobs(BlobTpl *blob_tpl, cv::UMat image);
	static cv::UMat markMatches(cv::UMat image, QVector<Match*> matches, cv::Scalar color = cv::Scalar(0, 0, 255), int thickness = 2);
	static cv::UMat markMatch(cv::UMat image, Match* match, cv::Scalar color = cv::Scalar(0, 0, 255), int thickness = 2);
	
	// Turns a QImage into a 3 Channel BGR cv::UMat
	static cv::UMat qimageToBGRUmat(const QImage &q_image);
};

#endif // BFP__VISION__VISION_H