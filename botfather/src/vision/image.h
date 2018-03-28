#ifndef BFP__VISION__IMAGE_H
#define BFP__VISION__IMAGE_H

#include <QObject>
#include <opencv2/opencv.hpp>

class Image : public QObject
{
Q_OBJECT
	
public:
	// Constructs an empty invalid image.
	Image();
	
	// Used to contruct a valid image (as long as umat is not empty).
	Image(cv::UMat umat);
	
	cv::UMat getUMat() const;
	
	Q_INVOKABLE bool valid();
	Q_INVOKABLE int getWidth();
	Q_INVOKABLE int getHeight();
	
private:
	cv::UMat m_umat;
};

#endif // BFP__VISION__IMAGE_H