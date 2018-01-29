#include "helper_api.h"
#include <QFileInfo>

HelperAPI::HelperAPI(QJSEngine* engine_p) : m_engine_p(engine_p)
{}

// static
void HelperAPI::enable(QJSEngine* engine_p)
{
	QJSValue vision_obj = engine_p->newQObject(new HelperAPI(engine_p));
	engine_p->globalObject().setProperty("Helper", vision_obj);
}

bool HelperAPI::fileExists(QString file_path)
{
	QFileInfo file(file_path);
	return file.exists() && file.isFile();
}