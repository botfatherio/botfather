#ifndef BROWSERHOST_H
#define BROWSERHOST_H

#include <QCoreApplication>
#include <QHash>
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

	// Shuts down CEF after closing all managed browsers.
	void quit();

	// Creates a new managed browser in the applications main thread no mather
	// which thread this method is called from. Managed browsers will be closed
	// before the application quits or you decide their group is no longer needed.
	CefRefPtr<CefBrowser> createManagedBrowser(const QString &group, const QString &name, const QSize &size);

	// Closes all managed browsers.
	void closeManagedBrowsers();

	// Closes all managed browsers in the specified group.
	void closeManagedBrowsers(const QString &group);

	// Closes all managed named browser in the specified group.
	void closeManagedNamedBrowsers(const QString &group);

	// Closes all managed unnamed browser in the specified group.
	void closeManagedUnnamedBrowsers(const QString &group);

protected:
	static void closeCefBrowser(CefRefPtr<CefBrowser> browser);

private:
	BrowserHost(){}
	CefSettings cefSettings() const;
	QTimer *m_cef_message_loop_timer = nullptr;
	QHash<QString, QHash<QString, CefRefPtr<CefBrowser>>> m_map_of_grouped_named_browsers;
	QHash<QString, QVector<CefRefPtr<CefBrowser>>> m_list_of_grouped_unnamed_browsers;
};

#endif // BROWSERHOST_H
