#ifndef BROWSER_PROTOTYPE_H
#define BROWSER_PROTOTYPE_H
#define BROWSER_PROTOTYPE_DOCS QString("https://botfather.io/docs/apiref/match/")

#include "abstract_prototype.h"
#include "../types/browser.h"

class BrowserPrototype : public AbstractPrototype
{
	Q_OBJECT

public:
	static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
	using AbstractPrototype::AbstractPrototype;

	Q_INVOKABLE QImage takeScreenshot();
	Q_INVOKABLE QSize getSize() const;
	Q_INVOKABLE QRect getRect() const;

	Q_INVOKABLE void blockResource(const QString &resource_pattern);
	Q_INVOKABLE void replaceResource(const QString &resource_pattern, const QString &replacement_url);
	Q_INVOKABLE void unmodifyResource(const QString &resource_pattern);
	Q_INVOKABLE void unmodifyResources();

	Q_INVOKABLE void loadUrl(const QString &url);
	Q_INVOKABLE void beOnUrl(const QString &url);
	Q_INVOKABLE QUrl getUrl() const;

	Q_INVOKABLE void reload();
	Q_INVOKABLE void reloadIgnoringCache();
	Q_INVOKABLE bool isLoading() const;
	Q_INVOKABLE bool finishLoading(int timeout_in_seconds = 30);
	Q_INVOKABLE void stopLoading();

	Q_INVOKABLE bool canGoBack() const;
	Q_INVOKABLE bool canGoForward() const;
	Q_INVOKABLE void goForward();
	Q_INVOKABLE void goBack();

	Q_INVOKABLE void leftClick(const QPoint &position);
	Q_INVOKABLE void middleClick(const QPoint &position);
	Q_INVOKABLE void rightClick(const QPoint &position);

	Q_INVOKABLE void pressLeft(const QPoint &position);
	Q_INVOKABLE void pressMiddle(const QPoint &position);
	Q_INVOKABLE void pressRight(const QPoint &position);

	Q_INVOKABLE void releaseLeft(const QPoint &position);
	Q_INVOKABLE void releaseMiddle(const QPoint &position);
	Q_INVOKABLE void releaseRight(const QPoint &position);
	Q_INVOKABLE void moveMouse(const QPoint &position);
	Q_INVOKABLE void scrollWheel(const QPoint &position, int delta_x, int delta_y);

	Q_INVOKABLE void executeJavascript(const QString &javascript_code);

	Q_INVOKABLE QString toString() const;
};

#endif // BROWSER_PROTOTYPE_H
