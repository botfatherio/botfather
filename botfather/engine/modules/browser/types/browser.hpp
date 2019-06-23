#ifndef BROWSER_H
#define BROWSER_H

#include <QObject>
#include <QMetaType>
#include <QUrl>
#include <QKeyEvent>
#include <QScriptValue>
#include <QCborValue>
#include "../browser_manager.hpp"
#include "../browser_client.hpp"

class Browser : public QObject
{
	Q_OBJECT

public:
	Browser(const QString &group, const QString &id, CefRefPtr<CefBrowser> cef_browser);
	Browser() {} // Required for metatype registration. Never use it

	CefRefPtr<CefBrowser> cefBrowser() const;
	BrowserClient *client() const;

	QString group() const;
	QString name() const;
	QSize size() const;
	QRect rect() const;
	QUrl url() const;

	QImage takeScreenshot();

	bool isLoading() const;
	bool finishLoading(int timeout_in_seconds = 30);

	bool canGoBack() const;
	bool canGoForward() const;

	void blockResource(const QString &resource_pattern);
	void replaceResource(const QString &resource_pattern, const QString &replacement_url);
	void unmodifyResource(const QString &resource_pattern);
	void unmodifyResources();

	void loadUrl(const QString &url);
	void beOnUrl(const QString &url);
	void goHome();

	void reload();
	void reloadIgnoringCache();
	void stopLoading();

	void goForward();
	void goBack();

	void executeJavascript(const QString &javascript_code);
	QCborValue evaluateJavascript(const QString &javascript_code);

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

	void enterText(const QString &text);

public slots:
	// Note: All public slots can be used by scripts.
	// We might want to introduce proxy objects to custom types at some point,
	// and reduce delegations in the Prototype files.
	void holdKey(const QKeyEvent *event);
	void releaseKey(const QKeyEvent *event);

signals:
	void aboutToGetDeletedByTheUser();

private:
	QString m_group;
	QString m_name;
	CefRefPtr<CefBrowser> m_cef_browser;
	BrowserClient *m_browser_client = nullptr;

	// A real keyboard provides us with all currently active modifiers on every key event.
	// A script controlling the browser via keyboard input doesn't.
	// Thus we have to store all previously applied keyboard modifiers.
	Qt::KeyboardModifiers m_unreleased_keyboard_modifiers;
};

#endif // BROWSER_H
