#ifndef BROWSER_H
#define BROWSER_H

#include <QObject>
#include <QMetaType>
#include <QUrl>
#include <QKeyEvent>
#include "../modules/browser/browser_manager.h"
#include "../modules/browser/browser_client.h"

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

public slots:
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

	void pressMouse(const QPoint &position, int qt_mouse_button);
	void releaseMouse(const QPoint &position, int qt_mouse_button);
	void moveMouse(const QPoint &position);
	void scrollWheel(const QPoint &position, const QPoint &delta);

	void holdKey(const QKeyEvent *event);
	void releaseKey(const QKeyEvent *event);
	void sendKeyEvent(const QKeyEvent *event, bool is_key_down);
	void sendKeyEvent(const CefKeyEvent &event, bool is_key_down);

signals:
	void aboutToGetDeletedByTheUser();

private:
	QString m_group;
	QString m_name;
	CefRefPtr<CefBrowser> m_cef_browser;
	BrowserClient *m_browser_client = nullptr;
};

Q_DECLARE_METATYPE(Browser*)

#endif // BROWSER_H
