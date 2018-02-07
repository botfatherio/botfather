#ifndef BFP__VISION__IMAGE_H
#define BFP__VISION__IMAGE_H

#include <QObject>
#include <opencv2/opencv.hpp>

class Image : public QObject
{
Q_OBJECT
	
public:
	explicit Image(cv::UMat umat);
	cv::UMat getUMat() const;
	
private:
	cv::UMat m_umat;
};

#endif // BFP__VISION__IMAGE_H