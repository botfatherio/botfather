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

	Q_INVOKABLE QString toString() const;
};

#endif // BFP_ENGINE_PROTOTYPES_BLOB_TPL_PROTOTYPE_H
