#ifndef BFP__BROWSER__BROWSER_API_H
#define BFP__BROWSER__BROWSER_API_H

#include <QObject>
#include <QScriptEngine>
#include <QString>

class Bot;

class BrowserAPI : public QObject
{
	Q_OBJECT
public:
	explicit BrowserAPI(Bot *bot_p, QScriptEngine* engine_p);
	static void enable(Bot *bot_p, QScriptEngine* engine_p);
	
	Q_INVOKABLE QScriptValue takeScreenshot();
	Q_INVOKABLE void blockRessource(QString ressource_pattern);
	Q_INVOKABLE void replaceRessource(QString old_ressource_pattern, QString new_ressource);
	Q_INVOKABLE void unmodifyRessource(QString ressource_pattern);
	Q_INVOKABLE void unmodifyRessources();
	Q_INVOKABLE void loadUrl(QString url);
	Q_INVOKABLE void beOnUrl(QString url);
	Q_INVOKABLE QString getUrl();
	Q_INVOKABLE void reload();
	Q_INVOKABLE void reloadIgnoringCache();
	Q_INVOKABLE bool loading();
	Q_INVOKABLE bool finishLoading(int timeout_seconds);
	Q_INVOKABLE void stopLoading();
	Q_INVOKABLE bool canGoBack();
	Q_INVOKABLE bool canGoForward();
	Q_INVOKABLE void goForward();
	Q_INVOKABLE void goBack();
	Q_INVOKABLE int getWidth();
	Q_INVOKABLE int getHeight();
	Q_INVOKABLE void executeJavascript(QString javascript_code);
	Q_INVOKABLE void leftClick(int x, int y);
	Q_INVOKABLE void middleClick(int x, int y);
	Q_INVOKABLE void rightClick(int x, int y);
	Q_INVOKABLE void pressLeft(int x, int y);
	Q_INVOKABLE void pressMiddle(int x, int y);
	Q_INVOKABLE void pressRight(int x, int y);
	Q_INVOKABLE void releaseLeft(int x, int y);
	Q_INVOKABLE void releaseMiddle(int x, int y);
	Q_INVOKABLE void releaseRight(int x, int y);
	Q_INVOKABLE void moveMouseTo(int x, int y);
	Q_INVOKABLE void scrollWheel(int x, int y, int delta_x, int delta_y);
	
private:
	QScriptEngine* m_engine_p;
};

#endif // BFP__BROWSER__BROWSER_API_H