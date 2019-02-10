#ifndef BROWSER_H
#define BROWSER_H

#include <QMetaType>
#include <QUrl>
#include "../modules/browser/browser_host.h"
#include "../modules/browser/browser_client.h"

class Browser
{
public:
	Browser(const QSize &size, const QString &name);
	Browser(const QSize &size);
	Browser(const QString &name);
	Browser();

	QImage takeScreenshot();
	QSize size() const;
	QRect rect() const;
	QString name() const;

	void loadUrl(const QString &url);
	void beOnUrl(const QString &url);
	QUrl url() const;

	void blockResource(const QString &resource_pattern);
	void replaceResource(const QString &resource_pattern, const QString &replacement_url);
	void unmodifyResource(const QString &resource_pattern);
	void unmodifyResources();

	void reload();
	void reloadIgnoringCache();
	bool isLoading() const;
	bool finishLoading(int timeout_in_seconds = 30);
	void stopLoading();

	bool canGoBack() const;
	bool canGoForward() const;
	void goForward();
	void goBack();

	void pressMouse(const CefBrowserHost::MouseButtonType &button_type, const QPoint &position);
	void releaseMouse(const CefBrowserHost::MouseButtonType &button_type, const QPoint &position);
	void moveMouse(const QPoint &position);
	void scrollWheel(const QPoint &position, int delta_x, int delta_y);

	void executeJavascript(const QString &javascript_code);

private:
	CefRefPtr<CefBrowser> m_cef_browser;
	BrowserClient *m_browser_client = nullptr;
};

Q_DECLARE_METATYPE(Browser)
Q_DECLARE_METATYPE(Browser*)

#endif // BROWSER_H
