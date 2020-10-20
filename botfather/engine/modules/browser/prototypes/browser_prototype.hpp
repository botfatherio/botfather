#ifndef BROWSER_PROTOTYPE_H
#define BROWSER_PROTOTYPE_H
#define BROWSER_PROTOTYPE_DOCS \
    QString("https://botfather.io/docs/apiref/browser/")

#include "../../common/prototypes/abstract_prototype.hpp"
#include "../types/browser.hpp"

class BrowserPrototype : public AbstractPrototype {
    Q_OBJECT
    Q_PROPERTY(QSize size READ getSize)
    Q_PROPERTY(QRect rect READ getRect)
    Q_PROPERTY(QUrl url READ getUrl)

   public:
    static QScriptValue constructor(QScriptContext *context,
                                    QScriptEngine *engine);
    using AbstractPrototype::AbstractPrototype;

    Q_INVOKABLE QImage takeScreenshot();
    Q_INVOKABLE QRect getRect() const;

    Q_INVOKABLE QSize getSize() const;
    Q_INVOKABLE void resize(const QSize &size);

    Q_INVOKABLE void blockResource(const QString &resource_pattern);
    Q_INVOKABLE void replaceResource(const QString &resource_pattern,
                                     const QString &replacement_url);
    Q_INVOKABLE void unmodifyResource(const QString &resource_pattern);
    Q_INVOKABLE void unmodifyResources();

    Q_INVOKABLE void setUserAgent(const QString &user_agent);
    Q_INVOKABLE void resetUserAgent();

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

    Q_INVOKABLE void holdLeft(const QPoint &position);
    Q_INVOKABLE void holdMiddle(const QPoint &position);
    Q_INVOKABLE void holdRight(const QPoint &position);

    Q_INVOKABLE void releaseLeft(const QPoint &position);
    Q_INVOKABLE void releaseMiddle(const QPoint &position);
    Q_INVOKABLE void releaseRight(const QPoint &position);

    Q_INVOKABLE void moveMouse(const QPoint &position);
    Q_INVOKABLE void scrollWheel(const QPoint &position, const QPoint &delta);

    Q_INVOKABLE void pressKey(const QString &keycode);
    Q_INVOKABLE void holdKey(const QString &keycode);
    Q_INVOKABLE void releaseKey(const QString &keycode);
    Q_INVOKABLE void enterText(const QString &text);

    Q_INVOKABLE void executeJavascript(const QString &javascript_code);
    Q_INVOKABLE QScriptValue evaluateJavascript(const QString &javascript_code,
                                                int timeout_in_ms = 10000);

    Q_INVOKABLE QString toString() const;
};

#endif  // BROWSER_PROTOTYPE_H
