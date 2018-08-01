#include "blob_tpl_factory.h"
#include "blob_tpl.h"
#include <QDebug>

BlobTplFactory::BlobTplFactory(QScriptEngine *engine_p)
	: m_engine_p(engine_p)
{}

// static
void BlobTplFactory::enable(QScriptEngine *engine_p)
{
	engine_p->globalObject().setProperty("BlobTpl", engine_p->newFunction(BlobTplFactory::create));
}

QScriptValue BlobTplFactory::create(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() < 2) {
		return context->throwError("Too few arguments.");
	}
	if (!context->argument(0).isQObject() || !qobject_cast<HSVColor*>(context->argument(0).toQObject())) {
		return context->throwError("Argument 'hsv_min' must be an HSVColor object.");
	}
	if (!context->argument(1).isQObject() || !qobject_cast<HSVColor*>(context->argument(1).toQObject())) {
		return context->throwError("Argument 'max_hsv' must be an HSVColor object.");
	}
	
	HSVColor *min_hsv = qobject_cast<HSVColor*>(context->argument(0).toQObject());
	HSVColor *max_hsv = qobject_cast<HSVColor*>(context->argument(1).toQObject());
	
	float min_area = context->argument(3).isNumber() ? static_cast<float>(context->argument(3).toNumber()) : -1;
	float max_area = context->argument(4).isNumber() ? static_cast<float>(context->argument(4).toNumber()) : -1;
	bool filter_by_area = min_area > 0 && max_area > 0 && context->argument(2).isBool() && context->argument(2).toBool();
	
	float min_circularity = context->argument(6).isNumber() ? static_cast<float>(context->argument(6).toNumber()) : -1;
	float max_circularity = context->argument(7).isNumber() ? static_cast<float>(context->argument(7).toNumber()) : -1;
	bool filter_by_circularity = min_circularity > 0 && max_circularity > 0 && context->argument(5).isBool() && context->argument(5).toBool();
	
	float min_convexity = context->argument(9).isNumber() ? static_cast<float>(context->argument(9).toNumber()) : -1;
	float max_convexity = context->argument(10).isNumber() ? static_cast<float>(context->argument(10).toNumber()) : -1;
	bool filter_by_convexity = min_convexity > 0 && max_convexity > 0 && context->argument(8).isBool() && context->argument(8).toBool();
	
	float min_inertia = context->argument(12).isNumber() ? static_cast<float>(context->argument(12).toNumber()) : -1;
	float max_inertia = context->argument(13).isNumber() ? static_cast<float>(context->argument(13).toNumber()) : -1;
	bool filter_by_inertia = min_inertia > 0 && max_inertia > 0 && context->argument(11).isBool() && context->argument(11).toBool();
	
	return engine->toScriptValue(new BlobTpl(
		min_hsv, max_hsv,
		filter_by_area, min_area, max_area,
		filter_by_circularity, min_circularity, max_circularity,
		filter_by_convexity, min_convexity, max_convexity,
		filter_by_inertia, min_inertia, max_inertia
	));
}