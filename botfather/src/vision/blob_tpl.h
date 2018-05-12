#ifndef BFP__VISION__BLOB_TPL_H
#define BFP__VISION__BLOB_TPL_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include "hsv_color.h"

class BlobTpl : public QObject
{
	Q_OBJECT
	
public:
	explicit BlobTpl(
		HSVColor* min_hsv_p,
		HSVColor* max_hsv_p,
		bool filter_by_area,
		int min_area,
		int max_area,
		bool filter_by_circularity = false,
		double min_circularity = 0.0f,
		double max_circularity = 1.0f,
		bool filter_by_convexity = false,
		double min_convexity = 0.0f,
		double max_convexity = 1.0f,
		bool filter_by_inertia = false,
		double min_inertia_ration = 0.0f,
		double max_inertia_ration = 1.0f
	);

	Q_INVOKABLE void setMinHSV(HSVColor* min_hsv_p);
	Q_INVOKABLE HSVColor* getMinHSV() const;
	
	Q_INVOKABLE void setMaxHSV(HSVColor* max_hsv_p);
	Q_INVOKABLE HSVColor* getMaxHSV() const;
	
	cv::SimpleBlobDetector::Params getBlobParams() const;
	
	Q_INVOKABLE void setFilterByArea(bool filter_by_area);
	Q_INVOKABLE bool getFilterByArea() const;
	
	Q_INVOKABLE void setMinArea(int min_area);
	Q_INVOKABLE int getMinArea() const;
	
	Q_INVOKABLE void setMaxArea(int max_area);
	Q_INVOKABLE int getMaxAarea() const;
	
	Q_INVOKABLE void setFilterByCircularity(bool filter_by_circularity);
	Q_INVOKABLE bool getFilterByCircularity() const;
	
	Q_INVOKABLE void setMinCircularity(double min_circularity);
	Q_INVOKABLE double getMinCircularity() const;
	
	Q_INVOKABLE void setMaxCircularity(double max_circularity);
	Q_INVOKABLE double getMaxCircularity() const;
	
	Q_INVOKABLE void setFilterByConvexity(bool filter_by_convexity);
	Q_INVOKABLE bool getFilterByConvexity() const;
	
	Q_INVOKABLE void setMinConvexity(double min_convexity);
	Q_INVOKABLE double getMinConvexity() const;
	
	Q_INVOKABLE void setMaxConvexity(double max_convexity);
	Q_INVOKABLE double getMaxConvexity() const;
	
	Q_INVOKABLE void setFilterByInertia(bool filter_by_inertia);
	Q_INVOKABLE bool getFilterByInertia() const;
	
	Q_INVOKABLE void setMinInertiaRation(double min_inertia_ration);
	Q_INVOKABLE double getMinInertiaRation() const;
	
	Q_INVOKABLE void setMaxInertiaRation(double max_inertia_ration);
	Q_INVOKABLE double getMaxInertiaRation() const;

private:
	HSVColor* m_min_hsv_p;
	HSVColor* m_max_hsv_p;
	cv::SimpleBlobDetector::Params m_blob_params;
};

#endif // BFP__VISION__BLOB_TPL_H