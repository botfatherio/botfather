#ifndef BROWSER_H
#define BROWSER_H

#include <QCborValue>
#include <QKeyEvent>
#include <QMetaType>
#include <QObject>
#include <QScriptValue>
#include <QUrl>

#include "../browser_client.hpp"
#include "../browser_manager.hpp"

class Browser : public QObject {
    Q_OBJECT

   public:
    Browser(const QString &group, const QString &id,
            CefRefPtr<CefBrowser> cef_browser);
    Browser() {}  // Required for metatype registration. Never use it

    CefRefPtr<CefBrowser> cefBrowser() const;
    BrowserClient *client() const;

    QString group() const;
    QString name() const;
    QRect rect() const;
    QUrl url() const;

    static const int MIN_WIDTH = 400;
    static const int MIN_HEIGHT = 400;

    QSize size() const;
    void resize(const QSize &size);

    QImage takeScreenshot();

    bool isLoading() const;
    bool finishLoading(int timeout_in_seconds = 30);

    bool canGoBack() const;
    bool canGoForward() const;

    void blockResource(const QString &resource_pattern);
    void replaceResource(const QString &resource_pattern,
                         const QString &replacement_url);
    void unmodifyResource(const QString &resource_pattern);
    void unmodifyResources();

    void setUserAgent(const QString &user_agent);
    void resetUserAgent();

    void loadUrl(const QString &url);
    void beOnUrl(const QString &url);
    void goHome();

    void reload();
    void reloadIgnoringCache();
    void stopLoading();

    void goForward();
    void goBack();

    void executeJavascript(const QString &javascript_code);
    bool evaluateJavascript(const QString &script_name,
                            const QString &javascript_code, int timeout_in_ms,
                            QCborValue &result, QVariantMap &exception,
                            bool &timed_out);

    void pressMouse(const QPoint &position, int qt_mouse_button);
    void holdMouse(const QPoint &position, int qt_mouse_button);
    void releaseMouse(const QPoint &position, int qt_mouse_button);

    void moveMouse(const QPoint &position);
    void scrollWheel(const QPoint &position, const QPoint &delta);

    bool canPressKey(const QString &bf_keycode) const;
    bool canEnterText(const QString &text) const;

    void pressKey(const QString &bf_keycode);
    void holdKey(const QString &bf_keycode);
    void releaseKey(const QString &bf_keycode);
    void holdKey(const CefKeyEvent &event);
    void releaseKey(const CefKeyEvent &event);
    void holdKey(const QKeyEvent *event);
    void releaseKey(const QKeyEvent *event);

    void enterText(const QString &text);

   signals:
    void aboutToGetDeletedByTheUser();

   private:
    QString m_group;
    QString m_name;
    CefRefPtr<CefBrowser> m_cef_browser;
    BrowserClient *m_browser_client = nullptr;

    // A real keyboard provides us with all currently active modifiers on every
    // key event. A script controlling the browser via keyboard input doesn't.
    // Thus we have to store all previously applied keyboard modifiers.
    Qt::KeyboardModifiers m_unreleased_keyboard_modifiers;
};

#endif  // BROWSER_H
