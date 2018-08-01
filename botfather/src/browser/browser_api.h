#ifndef BFP__BROWSER__BROWSER_API_H
#define BFP__BROWSER__BROWSER_API_H

#include <QObject>
#include <QScriptEngine>
#include <QString>

class Bot;
class Image;

class BrowserAPI : public QObject
{
	Q_OBJECT
public:
	explicit BrowserAPI(Bot *bot_p, QScriptEngine* engine_p);
	static void enable(Bot *bot_p, QScriptEngine* engine_p);
	
	Q_INVOKABLE QScriptValue takeScreenshot();
	Q_INVOKABLE void blockResource(QString resource_pattern);
	Q_INVOKABLE void replaceResource(QString old_resource_pattern, QString new_resource);
	Q_INVOKABLE void unmodifyResource(QString resource_pattern);
	Q_INVOKABLE void unmodifyResources();
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
	
	Q_INVOKABLE bool findAndClick(Image* tpl, double threshold = 0.8, int button = 1);
	Q_INVOKABLE QScriptValue findMatches(Image* tpl, double threshold = 0.8, int max_matches = 7);
	Q_INVOKABLE QScriptValue findMatch(Image* tpl, double threshold = 0.8);
	
private:
	Bot *m_bot_p;
	QScriptEngine* m_engine_p;
};

#endif // BFP__BROWSER__BROWSER_API_H