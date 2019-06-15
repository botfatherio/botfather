#ifndef BROWSERHOST_H
#define BROWSERHOST_H

#include <QCoreApplication>
#include <QHash>
#include <QTimer>
#include <include/cef_base.h>
#include "browser_app.hpp"
#include "browser_list_model.hpp"
#include "../../types/browser.hpp"

class BrowserManager
{
public:
	static BrowserManager *instance();
	BrowserManager(BrowserManager const&) = delete;
	void operator=(BrowserManager const&) = delete;

	BrowserListModel *model() const;

	// Spawns required subprocesses. This must be called right after starting the application.
	// Returns true when the initialisation succeeded otherwise false.
	bool init(int argc, char **argv);

	// Bind CEF and its message loop to the Qt application.
	void bind(QCoreApplication *app);

	// Shuts down CEF after closing all managed browsers.
	void quit();

	// Creates a new managed browser in the applications main thread no matter
	// which thread this method is called from.
	Browser *createBrowser(const QString &group_name, const QString &browser_id, const QSize &size);

	// Deletes a Browser created using the .createBrowser() method after removing it from the BrowserListModel
	// and correctly freeing the underlying CefBrowser pointer.
	void deleteBrowser(Browser *browser);

	// Closes all browsers created using .createBrowser()
	void deleteBrowsers();

protected:
	static void closeCefBrowser(CefRefPtr<CefBrowser> browser);

private:
	BrowserManager();
	CefSettings cefSettings() const;
	QTimer *m_cef_message_loop_timer = nullptr;
	BrowserListModel *m_model = nullptr;
};

#endif // BROWSERHOST_H
