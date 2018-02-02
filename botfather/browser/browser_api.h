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
	Q_INVOKABLE static bool loading();
	Q_INVOKABLE static bool bideLoading(int timeout_seconds);
	Q_INVOKABLE static void stopLoading();
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
	Q_INVOKABLE static void pressLeftAt(int x, int y);
	Q_INVOKABLE static void pressMiddleAt(int x, int y);
	Q_INVOKABLE static void pressRightAt(int x, int y);
	Q_INVOKABLE static void releaseLeftAt(int x, int y);
	Q_INVOKABLE static void releaseMiddleAt(int x, int y);
	Q_INVOKABLE static void releaseRightAt(int x, int y);
	Q_INVOKABLE static void moveMouseTo(int x, int y);
	Q_INVOKABLE static void scrollWheel(int x, int y, int delta_x, int delta_y);
	
private:
	QJSEngine* m_engine_p;
	QSettings m_settings;
};

#endif // BROWSER_API_H