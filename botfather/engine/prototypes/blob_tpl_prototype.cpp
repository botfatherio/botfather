#include "blob_tpl_prototype.h"

#define THIS_BLOB_TPL() qscriptvalue_cast<BlobTpl>(context()->thisObject())
#define THIS_BLOB_TPL_P() qscriptvalue_cast<BlobTpl*>(context()->thisObject())

QScriptValue BlobTplPrototype::constructor(QScriptContext *context, QScriptEngine *engine)
{
	for (int i = 0; i < context->argumentCount(); i++) {
		if (!context->argument(i).isNumber()) {
			// All arguments must be numbers
			NO_MATCHING_CTOR("BlobTpl", BLOB_TPL_PROTOTYPE_DOCS)
		}
	}
	
	if (context->argumentCount() > 10) {
		NO_MATCHING_CTOR("BlobTpl", BLOB_TPL_PROTOTYPE_DOCS)
	}
	
	// BlobTpl(min_area, max_area, min_circularity, max_circularity, min_convexity, max_convexity, min_inertia, max_inertia, min_threshold, max_threshold, threshold_step);
	BlobTpl blob_tpl;
	
	// min, max area
	if (context->argumentCount() >= 2)
	{
		int min_area = context->argument(0).toInt32();
		int max_area = context->argument(1).toInt32();
		
		if (min_area > max_area)
		{
			return RANGE_ERROR(context, "min_area must be smaller or equal to max_area");
		}
		
		blob_tpl.setMinArea(min_area);
		blob_tpl.setMaxArea(max_area);
	}
	
	// min, max curcularity
	if (context->argumentCount() >= 4)
	{
		double min_circularity = context->argument(2).toNumber();
		double max_circularity = context->argument(3).toNumber();
		
		if (min_circularity > max_circularity)
		{
			return RANGE_ERROR(context, "min_circularity must be smaller or equal to max_circularity");
		}
		if (max_circularity > 1.0)
		{
			return RANGE_ERROR(context, "max_circularity must be smaller or equal to 1.0");
		}
		
		blob_tpl.setMinCircularity(min_circularity);
		blob_tpl.setMaxCircularity(max_circularity);
	}
	
	// min, max convexity
	if (context->argumentCount() >= 6)
	{
		double min_convexity = context->argument(4).toNumber();
		double max_convexity = context->argument(5).toNumber();
		
		if (min_convexity > max_convexity)
		{
			return RANGE_ERROR(context, "min_convexity must be smaller or equal to max_convexity");
		}
		if (max_convexity > 1.0)
		{
			return RANGE_ERROR(context, "max_convexity must be smaller or equal to 1.0");
		}
		
		blob_tpl.setMinConvexity(min_convexity);
		blob_tpl.setMaxConvexity(max_convexity);
	}
	
	// min, max inertia
	if (context->argumentCount() >= 8)
	{
		double min_inertia = context->argument(6).toNumber();
		double max_inertia = context->argument(7).toNumber();
		
		if (min_inertia > max_inertia)
		{
			return RANGE_ERROR(context, "min_inertia must be smaller or equal to max_inertia");
		}
		if (max_inertia > 1.0)
		{
			return RANGE_ERROR(context, "max_inertia must be smaller or equal to 1.0");
		}
		
		blob_tpl.setMinInertia(min_inertia);
		blob_tpl.setMaxInertia(max_inertia);
	}
	
	// min, max threshold
	if (context->argumentCount() >= 10)
	{
		int min_threshold = context->argument(8).toInt32();
		int max_threshold = context->argument(9).toInt32();
		
		if (min_threshold > max_threshold)
		{
			return RANGE_ERROR(context, "min_threshold must be smaller or equal to max_threshold");
		}
		if (max_threshold > 255)
		{
			return RANGE_ERROR(context, "max_threshold must be smaller or equal to 255");
		}
		
		blob_tpl.setMinThreshold(min_threshold);
		blob_tpl.setMaxThreshold(max_threshold);
	}
	
	// threshold step
	if (context->argumentCount() == 11)
	{
		int threshold_step = context->argument(10).toInt32();
		int min_threshold = blob_tpl.getMinThreshold();
		int max_threshold = blob_tpl.getMaxThreshold();
		
		if (threshold_step < 1)
		{
			return RANGE_ERROR(context, "threshold_step must be atleast 1");
		}
		if (threshold_step > max_threshold - min_threshold)
		{
			return RANGE_ERROR(context, "threshold_step must be smaller or equal to max_circularity");
		}
		
		blob_tpl.setThresholdStep(threshold_step);
	}
	
	return engine->toScriptValue(blob_tpl);
}

void BlobTplPrototype::setMinArea(int new_min_area)
{
	THIS_BLOB_TPL_P()->setMinArea(new_min_area);
}

int BlobTplPrototype::getMinArea() const
{
	return THIS_BLOB_TPL().getMinArea();	
}

void BlobTplPrototype::setMaxArea(int new_max_area)
{
	THIS_BLOB_TPL_P()->setMaxArea(new_max_area);
}

int BlobTplPrototype::getMaxArea() const
{
	return THIS_BLOB_TPL().getMaxArea();	
}

void BlobTplPrototype::setMinCircularity(double new_min_circularity)
{
	THIS_BLOB_TPL_P()->setMinCircularity(new_min_circularity);
}

double BlobTplPrototype::getMinCircularity() const
{
	return THIS_BLOB_TPL().getMinCircularity();	
}

void BlobTplPrototype::setMaxCircularity(double new_max_circularity)
{
	THIS_BLOB_TPL_P()->setMaxCircularity(new_max_circularity);
}

double BlobTplPrototype::getMaxCircularity() const
{
	return THIS_BLOB_TPL().getMaxCircularity();	
}

void BlobTplPrototype::setMinConvexity(double new_min_convexity)
{
	THIS_BLOB_TPL_P()->setMinConvexity(new_min_convexity);
}

double BlobTplPrototype::getMinConvexity() const
{
	return THIS_BLOB_TPL().getMinConvexity();	
}

void BlobTplPrototype::setMaxConvexity(double new_max_convexity)
{
	THIS_BLOB_TPL_P()->setMaxConvexity(new_max_convexity);
}

double BlobTplPrototype::getMaxConvexity() const
{
	return THIS_BLOB_TPL().getMaxConvexity();	
}

void BlobTplPrototype::setMinInertia(double new_min_inertia_ratio)
{
	THIS_BLOB_TPL_P()->setMinInertia(new_min_inertia_ratio);
}

double BlobTplPrototype::getMinInertia() const
{
	return THIS_BLOB_TPL().getMinInertia();	
}

void BlobTplPrototype::setMaxInertia(double new_max_inertia_ratio)
{
	THIS_BLOB_TPL_P()->setMaxInertia(new_max_inertia_ratio);
}

double BlobTplPrototype::getMaxInertia() const
{
	return THIS_BLOB_TPL().getMaxInertia();	
}

void BlobTplPrototype::setMinThreshold(int new_min_threshold)
{
	THIS_BLOB_TPL_P()->setMinThreshold(new_min_threshold);
}

int BlobTplPrototype::getMinThreshold() const
{
	return THIS_BLOB_TPL().getMinThreshold();	
}

void BlobTplPrototype::setMaxThreshold(int new_max_threshold)
{
	THIS_BLOB_TPL_P()->setMaxThreshold(new_max_threshold);
}

int BlobTplPrototype::getMaxThreshold() const
{
	return THIS_BLOB_TPL().getMaxThreshold();	
}

void BlobTplPrototype::setThresholdStep(int new_threshold_step)
{
	THIS_BLOB_TPL_P()->setThresholdStep(new_threshold_step);
}

int BlobTplPrototype::getThresholdStep() const
{
	return THIS_BLOB_TPL().getThresholdStep();	
}

QString BlobTplPrototype::toString() const
{
	BlobTpl blob_tpl = THIS_BLOB_TPL();
	return QString("BlobTpl(area: %1-%2, circ: %3-%4, conv: %5-%6, iner: %7-%8, thre: %9-%10, step: %11)")
		.arg(blob_tpl.getMinArea()).arg(blob_tpl.getMaxArea())
		.arg(blob_tpl.getMinCircularity()).arg(blob_tpl.getMaxCircularity())
		.arg(blob_tpl.getMinConvexity()).arg(blob_tpl.getMaxConvexity())
		.arg(blob_tpl.getMinInertia()).arg(blob_tpl.getMaxInertia())
		.arg(blob_tpl.getMinThreshold()).arg(blob_tpl.getMaxThreshold())
		.arg(blob_tpl.getThresholdStep());
}
