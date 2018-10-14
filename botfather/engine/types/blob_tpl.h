#ifndef BFP_ENGINE_TYPES_BLOB_TPL_H
#define BFP_ENGINE_TYPES_BLOB_TPL_H

#include <QMetaType>
#include <QColor>
#include <opencv2/features2d.hpp>

class BlobTpl
{
public:
	BlobTpl();
	cv::SimpleBlobDetector::Params getBlobParams() const;

	void setMinArea(int new_min_area);
	int getMinArea() const;
	
	void setMaxArea(int new_max_area);
	int getMaxArea() const;
	
	void setMinCircularity(double new_min_circularity);
	double getMinCircularity() const;
	
	void setMaxCircularity(double new_max_circularity);
	double getMaxCircularity() const;
	
	void setMinConvexity(double new_min_convexity);
	double getMinConvexity() const;
	
	void setMaxConvexity(double new_max_convexity);
	double getMaxConvexity() const;
	
	void setMinInertia(double new_min_inertia_ratio);
	double getMinInertia() const;
	
	void setMaxInertia(double new_max_inertia_ratio);
	double getMaxInertia() const;
	
	void setMinThreshold(int new_min_threshold);
	int getMinThreshold() const;
	
	void setMaxThreshold(int new_max_threshold);
	int getMaxThreshold() const;
	
	void setThresholdStep(int new_threshold_step);
	int getThresholdStep() const;
	
	void setMinBlobDistance(int new_min_blob_distance);
	int getMinBlobDistance() const;
	
	void setMinRepeatability(int new_min_repeatability);
	int getMinRepeatability() const;
	
private:
	cv::SimpleBlobDetector::Params blob_params;
};

Q_DECLARE_METATYPE(BlobTpl)
Q_DECLARE_METATYPE(BlobTpl*)

#endif // BFP_ENGINE_TYPES_BLOB_TPL_H