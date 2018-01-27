#include "image.h"

Image::Image(cv::UMat umat)
	: m_umat(umat)
{}

cv::UMat Image::getUMat() const
{
	return this->m_umat;
}