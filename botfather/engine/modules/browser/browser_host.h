#ifndef BROWSERHOST_H
#define BROWSERHOST_H

#include <QCoreApplication>
#include <QTimer>
#include <include/cef_base.h>
#include "browser_app.h"

class BrowserHost // FIXME: maybe call this BrowserManager or BrowserHive to avoid confusing with CefBrowserHost
{
public:
	static BrowserHost *instance();
	BrowserHost(BrowserHost const&) = delete;
	void operator=(BrowserHost const&) = delete;

	// Spawns required subprocesses. This must be called right after starting the application.
	// Returns true when the initialisation succeeded otherwise false.
	bool init(int argc, char **argv);

	// Bind CEF and its message loop to the Qt application.
	void bind(QCoreApplication *app);

	CefRefPtr<CefBrowser> createManagedBrowser(const QSize &size, const QString &name);

private:
	BrowserHost(){}
	CefSettings cefSettings() const;
	QTimer *m_cef_message_loop_timer = nullptr;
};

#endif // BROWSERHOST_H
