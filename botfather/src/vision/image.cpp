#include "image.h"

Image::Image()
{}

Image::Image(cv::UMat umat)
	: m_umat(umat)
{}

cv::UMat Image::getUMat() const
{
	return this->m_umat;
}

bool Image::isValid()
{
	return !this->m_umat.empty();
}

int Image::getWidth()
{
	return m_umat.cols;
}

int Image::getHeight()
{
	return m_umat.rows;
}