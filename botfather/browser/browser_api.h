#ifndef BROWSER_API_H
#define BROWSER_API_H

#include <QObject>
#include <QJSEngine>
#include <QString>
#include <QSettings>

class BrowserAPI : public QObject
{
	Q_OBJECT
public:
	explicit BrowserAPI(QJSEngine* engine_p);
	static void enable(QJSEngine* engine_p);
	
	Q_INVOKABLE static void blockRessource(QString ressource);
	Q_INVOKABLE static void replaceRessource(QString old_ressource, QString new_ressource);
	Q_INVOKABLE static void unmodifyRessource(QString ressource);
	Q_INVOKABLE static void unmodifyRessources();
	Q_INVOKABLE static void loadUrl(QString url);
	Q_INVOKABLE static QString getUrl();
	Q_INVOKABLE static void reload();
	Q_INVOKABLE static void reloadIgnoringCache();
	Q_INVOKABLE static void stopLoad();
	Q_INVOKABLE static bool canGoBack();
	Q_INVOKABLE static bool canGoForward();
	Q_INVOKABLE static void goForward();
	Q_INVOKABLE static void goBack();
	Q_INVOKABLE static int getWidth();
	Q_INVOKABLE static int getHeight();
	Q_INVOKABLE static void executeJavascript(QString javascript_code);
	Q_INVOKABLE static void leftClickAt(int x, int y);
	Q_INVOKABLE static void middleClickAt(int x, int y);
	Q_INVOKABLE static void rightClickAt(int x, int y);
	Q_INVOKABLE static void warpMouseTo(int x, int y);
	
private:
	QJSEngine* m_engine_p;
	QSettings m_settings;
};

#endif // BROWSER_API_H