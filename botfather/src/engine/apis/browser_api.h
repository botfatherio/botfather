#ifndef BFP_ENGINE_APIS_BROWSER_API_H
#define BFP_ENGINE_APIS_BROWSER_API_H

#include <QObject>
#include <QScriptEngine>
#include <QImage>
#include <QString>
#include <QUrl>
#include "abstract_api.h"

class BrowserAPI : public AbstractAPI
{
	Q_OBJECT
public:
	using AbstractAPI::AbstractAPI;
	
	Q_INVOKABLE QImage takeScreenshot();

	Q_INVOKABLE void blockResource(const QString &resource_pattern);
	Q_INVOKABLE void replaceResource(const QString &old_resource_pattern, const QString &new_resource);
	Q_INVOKABLE void unmodifyResource(const QString &resource_pattern);
	Q_INVOKABLE void unmodifyResources();

	Q_INVOKABLE void loadUrl(const QString &url);
	Q_INVOKABLE void beOnUrl(const QString &url);
	Q_INVOKABLE QUrl getUrl();

	Q_INVOKABLE void reload();
	Q_INVOKABLE void reloadIgnoringCache();
	Q_INVOKABLE bool loading();
	Q_INVOKABLE bool finishLoading(int timeout_seconds = 30);
	Q_INVOKABLE void stopLoading();

	Q_INVOKABLE bool canGoBack();
	Q_INVOKABLE bool canGoForward();
	Q_INVOKABLE void goForward();
	Q_INVOKABLE void goBack();

	Q_INVOKABLE QSize getSize();
	Q_INVOKABLE QRect getRect();

	Q_INVOKABLE void executeJavascript(const QString &javascript_code);

	Q_INVOKABLE void leftClick(const QPoint &position);
	Q_INVOKABLE void middleClick(const QPoint &position);
	Q_INVOKABLE void rightClick(const QPoint &position);

	Q_INVOKABLE void pressLeft(const QPoint &position);
	Q_INVOKABLE void pressMiddle(const QPoint &position);
	Q_INVOKABLE void pressRight(const QPoint &position);

	Q_INVOKABLE void releaseLeft(const QPoint &position);
	Q_INVOKABLE void releaseMiddle(const QPoint &position);
	Q_INVOKABLE void releaseRight(const QPoint &position);

	Q_INVOKABLE void moveMouseTo(const QPoint &position);
	Q_INVOKABLE void scrollWheel(const QPoint &position, int delta_x, int delta_y);
	
	Q_INVOKABLE bool findAndClick(const QImage &tpl, double threshold = 0.8, int button = 1);
	Q_INVOKABLE QScriptValue findMatches(const QImage &tpl, double threshold = 0.8, int max_matches = -1);
	Q_INVOKABLE QScriptValue findMatch(const QImage &tpl, double threshold = 0.8);
};

#endif // BFP_ENGINE_APIS_BROWSER_API_H
