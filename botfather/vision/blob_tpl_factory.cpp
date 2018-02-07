#include "blob_tpl_factory.h"
#include "blob_tpl.h"

BlobTplFactory::BlobTplFactory(QJSEngine *engine_p)
	: m_engine_p(engine_p)
{}

QJSValue BlobTplFactory::createInstance(
	HSVColor* min_hsv,
	HSVColor* max_hsv,
	bool filter_by_area,
	int min_area,
	int max_area,
	bool filter_by_circularity,
	double min_circularity,
	double max_circularity,
	bool filter_by_convexity,
	double min_convexity,
	double max_convexity,
	bool filter_by_inertia,
	double min_inertia_ration,
	double max_inertia_ration)
{
	// The engine takes ownership and destroys the object if no longer required.
	return m_engine_p->newQObject(new BlobTpl(
		min_hsv,
		max_hsv,
		filter_by_area,
		min_area,
		max_area,
		filter_by_circularity,
		min_circularity,
		max_circularity,
		filter_by_convexity,
		min_convexity,
		max_convexity,
		filter_by_inertia,
		min_inertia_ration,
		max_inertia_ration 
	));
}

// static
void BlobTplFactory::enable(QJSEngine *engine_p)
{
	QJSValue hsv_color_factory = engine_p->newQObject(new BlobTplFactory(engine_p));
	engine_p->globalObject().setProperty("_blob_tpl_factory", hsv_color_factory);
	engine_p->evaluate(
		"function BlobTpl("
			"min_hsv,"
			"max_hsv,"
			"filter_by_area,"
			"min_area,"
			"max_area,"
			"filter_by_circularity,"
			"min_circularity,"
			"max_circularity,"
			"filter_by_convexity,"
			"min_convexity,"
			"max_convexity,"
			"filter_by_inertia,"
			"min_inertia_ration,"
			"max_inertia_ration)"
		"{"
			//"if (typeof(min_hsv)==='undefined') min_hsv"
			//"if (typeof(max_hsv)==='undefined') max_hsv"
			//"if (typeof(filter_by_area)==='undefined') filter_by_area"
			"if (typeof(min_area)==='undefined') min_area = 0.0;"
			"if (typeof(max_area)==='undefined') max_area = 0.0;"
			"if (typeof(filter_by_circularity)==='undefined') filter_by_circularity = false;"
			"if (typeof(min_circularity)==='undefined') min_circularity = 0.0;"
			"if (typeof(max_circularity)==='undefined') max_circularity = 0.0;"
			"if (typeof(filter_by_convexity)==='undefined') filter_by_convexity = false;"
			"if (typeof(min_convexity)==='undefined') min_convexity = 0.0;"
			"if (typeof(max_convexity)==='undefined') max_convexity = 0.0;"
			"if (typeof(filter_by_inertia)==='undefined') filter_by_inertia = false;"
			"if (typeof(min_inertia_ration)==='undefined') min_inertia_ration = 0.0;"
			"if (typeof(max_inertia_ration)==='undefined') max_inertia_ration = 0.0;"
			""
			"return _blob_tpl_factory.createInstance("
				"min_hsv,"
				"max_hsv,"
				"filter_by_area,"
				"min_area,"
				"max_area,"
				"filter_by_circularity,"
				"min_circularity,"
				"max_circularity,"
				"filter_by_convexity,"
				"min_convexity,"
				"max_convexity,"
				"filter_by_inertia,"
				"min_inertia_ration,"
				"max_inertia_ration"
			");"
		"}"
	);
}