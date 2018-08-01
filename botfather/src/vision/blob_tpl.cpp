#include "blob_tpl.h"

BlobTpl::BlobTpl(	
	HSVColor* min_hsv_p,
	HSVColor* max_hsv_p,
	bool filter_by_area,
	float min_area,
	float max_area,
	bool filter_by_circularity,
	float min_circularity,
	float max_circularity,
	bool filter_by_convexity,
	float min_convexity,
	float max_convexity,
	bool filter_by_inertia,
	float min_inertia_ration,
	float max_inertia_ration)
{
	this->m_min_hsv_p = min_hsv_p;
	this->m_max_hsv_p = max_hsv_p;
	this->m_blob_params.filterByColor = true;
	this->m_blob_params.blobColor = 255; // Our threshold will have white spots/blobs to detect.
	this->m_blob_params.filterByArea = filter_by_area;
	this->m_blob_params.minArea = min_area;
	this->m_blob_params.maxArea = max_area;
	this->m_blob_params.filterByCircularity = filter_by_circularity;
	this->m_blob_params.minCircularity = min_circularity;
	this->m_blob_params.maxCircularity = max_circularity;
	this->m_blob_params.filterByConvexity = filter_by_convexity;
	this->m_blob_params.minConvexity = min_convexity;
	this->m_blob_params.maxConvexity = max_convexity;
	this->m_blob_params.filterByInertia = filter_by_inertia;
	this->m_blob_params.minInertiaRatio = min_inertia_ration;
	this->m_blob_params.maxInertiaRatio = max_inertia_ration;
}

QScriptValue BlobTpl::toScriptValue(QScriptEngine *engine, BlobTpl* const &in)
{
	return engine->newQObject(in);
}

void BlobTpl::fromScriptValue(const QScriptValue &object, BlobTpl* &out)
{
	out = qobject_cast<BlobTpl*>(object.toQObject());
}

void BlobTpl::setMinHSV(HSVColor *min_hsv_p)
{
	this->m_min_hsv_p = min_hsv_p;
}

HSVColor* BlobTpl::getMinHSV() const
{
	return this->m_min_hsv_p;
}

void BlobTpl::setMaxHSV(HSVColor *max_hsv_p)
{
	this->m_max_hsv_p = max_hsv_p;
}

HSVColor* BlobTpl::getMaxHSV() const
{
	return this->m_max_hsv_p;
}

cv::SimpleBlobDetector::Params BlobTpl::getBlobParams() const
{
	return this->m_blob_params;
}

void BlobTpl::setFilterByArea(bool filter_by_area)
{
	this->m_blob_params.filterByArea = filter_by_area;
}

bool BlobTpl::getFilterByArea() const
{
	return this->m_blob_params.filterByArea;
}

void BlobTpl::setMinArea(float min_area)
{
	this->m_blob_params.minArea = min_area;
}

float BlobTpl::getMinArea() const
{
	return this->m_blob_params.minArea;
}

void BlobTpl::setMaxArea(float max_area)
{
	this->m_blob_params.maxArea = max_area;
}

float BlobTpl::getMaxAarea() const
{
	return this->m_blob_params.maxArea;
}

void BlobTpl::setFilterByCircularity(bool filter_by_circularity)
{
	this->m_blob_params.filterByCircularity = filter_by_circularity;
}

bool BlobTpl::getFilterByCircularity() const
{
	return this->m_blob_params.filterByCircularity;
}

void BlobTpl::setMinCircularity(float min_circularity)
{
	this->m_blob_params.minCircularity = min_circularity;
}

float BlobTpl::getMinCircularity() const
{
	return this->m_blob_params.minCircularity;
}

void BlobTpl::setMaxCircularity(float max_circularity)
{
	this->m_blob_params.maxCircularity = max_circularity;
}

float BlobTpl::getMaxCircularity() const
{
	return this->m_blob_params.maxCircularity;
}

void BlobTpl::setFilterByConvexity(bool filter_by_convexity)
{
	this->m_blob_params.filterByConvexity = filter_by_convexity;
}

bool BlobTpl::getFilterByConvexity() const
{
	return this->m_blob_params.filterByConvexity;
}

void BlobTpl::setMinConvexity(float min_convexity)
{
	this->m_blob_params.minConvexity = min_convexity;
}

float BlobTpl::getMinConvexity() const
{
	return this->m_blob_params.minConvexity;
}

void BlobTpl::setMaxConvexity(float max_convexity)
{
	this->m_blob_params.maxConvexity = max_convexity;
}

float BlobTpl::getMaxConvexity() const
{
	return this->m_blob_params.maxConvexity;
}

void BlobTpl::setFilterByInertia(bool filter_by_inertia)
{
	this->m_blob_params.filterByInertia = filter_by_inertia;
}

bool BlobTpl::getFilterByInertia() const
{
	return this->m_blob_params.filterByInertia;
}

void BlobTpl::setMinInertiaRation(float min_inertia)
{
	this->m_blob_params.minInertiaRatio = min_inertia;
}

float BlobTpl::getMinInertiaRation() const
{
	return this->m_blob_params.minInertiaRatio;
}

void BlobTpl::setMaxInertiaRation(float max_inertia)
{
	this->m_blob_params.maxInertiaRatio = max_inertia;
}

float BlobTpl::getMaxInertiaRation() const
{
	return this->m_blob_params.maxInertiaRatio;
}