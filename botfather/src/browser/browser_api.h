#ifndef BFP__BROWSER__BROWSER_API_H
#define BFP__BROWSER__BROWSER_API_H

#include <QObject>
#include <QJSEngine>
#include <QString>

class Bot;

class BrowserAPI : public QObject
{
	Q_OBJECT
public:
	explicit BrowserAPI(Bot *bot_p, QJSEngine* engine_p);
	static void enable(Bot *bot_p, QJSEngine* engine_p);
	
	Q_INVOKABLE QJSValue getImage();
	Q_INVOKABLE void blockRessource(QString ressource_pattern);
	Q_INVOKABLE void replaceRessource(QString old_ressource_pattern, QString new_ressource);
	Q_INVOKABLE void unmodifyRessource(QString ressource_pattern);
	Q_INVOKABLE void unmodifyRessources();
	Q_INVOKABLE void loadUrl(QString url);
	Q_INVOKABLE QString getUrl();
	Q_INVOKABLE void reload();
	Q_INVOKABLE void reloadIgnoringCache();
	Q_INVOKABLE bool loading();
	Q_INVOKABLE bool bideLoading(int timeout_seconds);
	Q_INVOKABLE void stopLoading();
	Q_INVOKABLE bool canGoBack();
	Q_INVOKABLE bool canGoForward();
	Q_INVOKABLE void goForward();
	Q_INVOKABLE void goBack();
	Q_INVOKABLE int getWidth();
	Q_INVOKABLE int getHeight();
	Q_INVOKABLE void executeJavascript(QString javascript_code);
	Q_INVOKABLE void leftClickAt(int x, int y);
	Q_INVOKABLE void middleClickAt(int x, int y);
	Q_INVOKABLE void rightClickAt(int x, int y);
	Q_INVOKABLE void pressLeftAt(int x, int y);
	Q_INVOKABLE void pressMiddleAt(int x, int y);
	Q_INVOKABLE void pressRightAt(int x, int y);
	Q_INVOKABLE void releaseLeftAt(int x, int y);
	Q_INVOKABLE void releaseMiddleAt(int x, int y);
	Q_INVOKABLE void releaseRightAt(int x, int y);
	Q_INVOKABLE void moveMouseTo(int x, int y);
	Q_INVOKABLE void scrollWheel(int x, int y, int delta_x, int delta_y);
	
private:
	QJSEngine* m_engine_p;
};

#endif // BFP__BROWSER__BROWSER_API_H