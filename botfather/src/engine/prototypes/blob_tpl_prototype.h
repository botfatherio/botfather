#ifndef BFP_ENGINE_PROTOTYPES_BLOB_TPL_PROTOTYPE_H
#define BFP_ENGINE_PROTOTYPES_BLOB_TPL_PROTOTYPE_H
#define BLOB_TPL_PROTOTYPE_DOCS QString("https://botfather.io/docs/apiref/blobtpl/")

#include "abstract_prototype.h"
#include "../types/blob_tpl.h"

class BlobTplPrototype : public AbstractPrototype
{
	Q_OBJECT
	
public:
	static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
	using AbstractPrototype::AbstractPrototype;
	
	// 0 <= min <= max
	void setMinArea(int new_min_area);
	int getMinArea() const;
	
	void setMaxArea(int new_max_area);
	int getMaxArea() const;
	
	// 0 <= min <= max <= 1.0f
	void setMinCircularity(double new_min_circularity);
	double getMinCircularity() const;

	void setMaxCircularity(double new_max_circularity);
	double getMaxCircularity() const;
	
	// 0 <= min <= max <= 1.0f
	void setMinConvexity(double new_min_convexity);
	double getMinConvexity() const;
	
	void setMaxConvexity(double new_max_convexity);
	double getMaxConvexity() const;
	
	// 0 <= min <= max <= 1.0f
	void setMinInertia(double new_min_inertia_ratio);
	double getMinInertia() const;
	
	void setMaxInertia(double new_max_inertia_ratio);
	double getMaxInertia() const;
	
	// 0 <= min <= max <= 255
	void setMinThreshold(int new_min_threshold);
	int getMinThreshold() const;
	
	void setMaxThreshold(int new_max_threshold);
	int getMaxThreshold() const;
	
	// 1 <= new_threshold_step <= max threshold - min threshold
	void setThresholdStep(int new_threshold_step);
	int getThresholdStep() const;
	
	
	
	
	
	
	
	
	/*
	// Sets the blobs area to new_area
	Q_INVOKABLE void setArea(int new_area);
	
	// Returns the blobs area
	Q_INVOKABLE int getArea() const;
	
	// Sets the blobs circularity to new_circularity. 0 <= new_circularity <= 1.0f
	Q_INVOKABLE void setCircularity(float new_circularity);
	
	// Returns the blobs circularity
	Q_INVOKABLE float getCircularity() const;
		
	// Sets the blobs convexity to new_convexity. 0 <= new_convexity <= 1.0f
	Q_INVOKABLE void setConvexity(float new_convexity);
	
	// Returns the blobs convexity
	Q_INVOKABLE float getConvexity() const;
	
	// Sets the blobs inertia ratio to new_inertia. 0 <= new_inertia_ratio <= 1.0f
	Q_INVOKABLE void setInertiaRatio(float new_inertia_ratio);
	
	// Returns the blobs inertia ratio
	Q_INVOKABLE float getInertiaRatio() const;
	
	// 0 <= new_threshold <= 255
	Q_INVOKABLE void setThreshold(int new_threshold);
	Q_INVOKABLE int getThreshold() const;
	*/
	
	Q_INVOKABLE QString toString() const;
};

#endif // BFP_ENGINE_PROTOTYPES_BLOB_TPL_PROTOTYPE_H