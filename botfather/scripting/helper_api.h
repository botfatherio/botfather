#ifndef BFP__MISC__HELPER_API_H
#define BFP__MISC__HELPER_API_H

#include <QObject>
#include <QJSEngine>
#include <QString>

class HelperAPI : public QObject
{
	Q_OBJECT
	
public:
	HelperAPI(QJSEngine* engine_p);
	static void enable(QJSEngine* engine_p);
	Q_INVOKABLE static bool fileExists(QString file_path);
	
private:
	QJSEngine* m_engine_p;
};

#endif // BFP__MISC__HELPER_API_H