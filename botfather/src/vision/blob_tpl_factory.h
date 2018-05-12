#ifndef BFP__VISION__BLOB_TPL_FACTORY_H
#define BFP__VISION__BLOB_TPL_FACTORY_H

#include <QObject>
#include <QString>
#include <QtScript/QScriptEngine>
#include "hsv_color.h"

class BlobTplFactory : public QObject
{
	Q_OBJECT
	
public:
	BlobTplFactory(QScriptEngine* engine_p);
	
	Q_INVOKABLE QScriptValue createInstance(
		HSVColor* min_hsv,
		HSVColor* max_hsv,
		bool filter_by_area,
		int min_area = 0.0,
		int max_area = 0.0,
		bool filter_by_circularity = false,
		double min_circularity = 0.0,
		double max_circularity = 1.0,
		bool filter_by_convexity = false,
		double min_convexity = 0.0,
		double max_convexity = 1.0,
		bool filter_by_inertia = false,
		double min_inertia_ration = 0.0,
		double max_inertia_ration = 1.0
	);
	
	static void enable(QScriptEngine* engine_p);

private:
	QScriptEngine* m_engine_p;
};

#endif // BFP__VISION__BLOB_TPL_FACTORY_H