#ifndef BFP__VISION__BLOB_TPL_H
#define BFP__VISION__BLOB_TPL_H

#include <QObject>
#include <QScriptEngine>
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
		float min_area,
		float max_area,
		bool filter_by_circularity = false,
		float min_circularity = 0.0f,
		float max_circularity = 1.0f,
		bool filter_by_convexity = false,
		float min_convexity = 0.0f,
		float max_convexity = 1.0f,
		bool filter_by_inertia = false,
		float min_inertia_ration = 0.0f,
		float max_inertia_ration = 1.0f
	);
	
	static QScriptValue toScriptValue(QScriptEngine *engine, BlobTpl* const &in);
	static void fromScriptValue(const QScriptValue &object, BlobTpl* &out);

	Q_INVOKABLE void setMinHSV(HSVColor* min_hsv_p);
	Q_INVOKABLE HSVColor* getMinHSV() const;
	
	Q_INVOKABLE void setMaxHSV(HSVColor* max_hsv_p);
	Q_INVOKABLE HSVColor* getMaxHSV() const;
	
	cv::SimpleBlobDetector::Params getBlobParams() const;
	
	Q_INVOKABLE void setFilterByArea(bool filter_by_area);
	Q_INVOKABLE bool getFilterByArea() const;
	
	Q_INVOKABLE void setMinArea(float min_area);
	Q_INVOKABLE float getMinArea() const;
	
	Q_INVOKABLE void setMaxArea(float max_area);
	Q_INVOKABLE float getMaxAarea() const;
	
	Q_INVOKABLE void setFilterByCircularity(bool filter_by_circularity);
	Q_INVOKABLE bool getFilterByCircularity() const;
	
	Q_INVOKABLE void setMinCircularity(float min_circularity);
	Q_INVOKABLE float getMinCircularity() const;
	
	Q_INVOKABLE void setMaxCircularity(float max_circularity);
	Q_INVOKABLE float getMaxCircularity() const;
	
	Q_INVOKABLE void setFilterByConvexity(bool filter_by_convexity);
	Q_INVOKABLE bool getFilterByConvexity() const;
	
	Q_INVOKABLE void setMinConvexity(float min_convexity);
	Q_INVOKABLE float getMinConvexity() const;
	
	Q_INVOKABLE void setMaxConvexity(float max_convexity);
	Q_INVOKABLE float getMaxConvexity() const;
	
	Q_INVOKABLE void setFilterByInertia(bool filter_by_inertia);
	Q_INVOKABLE bool getFilterByInertia() const;
	
	Q_INVOKABLE void setMinInertiaRation(float min_inertia_ration);
	Q_INVOKABLE float getMinInertiaRation() const;
	
	Q_INVOKABLE void setMaxInertiaRation(float max_inertia_ration);
	Q_INVOKABLE float getMaxInertiaRation() const;

private:
	HSVColor* m_min_hsv_p; // NOTE: Script owned
	HSVColor* m_max_hsv_p; // NOTE: Script owned
	cv::SimpleBlobDetector::Params m_blob_params;
};

Q_DECLARE_METATYPE(BlobTpl*)

#endif // BFP__VISION__BLOB_TPL_H