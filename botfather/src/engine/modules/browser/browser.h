#ifndef BFP_ENGINE_MODULES_BROWSER_BROWSER_H
#define BFP_ENGINE_MODULES_BROWSER_BROWSER_H

#include <QString>
#include <QUrl>
#include <QImage>
#include <include/cef_base.h>

class Browser
{
public:
	// Calling init or quit more than once is not allowed. Also init has to called
	// first and quit before ending the program.
	static void init(int argc, char **argv);
	static void quit();

	// Resizes the browser
	static void resize(const QSize &new_size);
	
	// Returns screenshot of browser as QImage
	static QImage takeScreenshot();
	
	// Adds a resource to the block list. Blocked resources are not loaded.
	static void blockResource(const QString &resource);
	
	// Makes the browser load a custom resource instead of the original.
	static void replaceResource(const QString &old_resource, const QString &new_resource);
	
	// Resets block and replace settings for a resource.
	static void unmodifyResource(const QString &resource);
	
	// Resets block and replace settings for all resources.
	static void unmodifyResources();
	
	// Load the given url/website.
	static void loadUrl(const QString &url);
	
	// Returns the currently loaded url.
	static QUrl getUrl();

	// Reloads the current page ignoring any cached data. 
	static void reloadIgnoringCache();
	
	// Returns true when the browser is loading.
	static bool loading();
	
	// Blocks until the browser stopped loading or the timeour expired. Returns false
	// when the timeout expired.
	static bool finishLoading(int timeout_seconds);
	
	// Returns true if the browser can navigate backwards.
	static bool canGoBack();
	
	// Returns true if the browser can navigate forwards.
	static bool canGoForward();
	
	// Returns the browsers current size.
	static QSize getSize();
	
	// Executes the given javascript on the currently loaded website (non-blocking).
	static void executeJavascript(const QString &javascript_code);
	
	// Sends a mouse press event to position
	static void pressMouse(int button_type, const QPoint &position);
	
	// Sends a mouse release event to position
	static void releaseMouse(int button_type, const QPoint &position);
	
	// Sends a mouse move event to position
	static void moveMouse(const QPoint &position);

	// Sends a mouse wheel event to the desired xy coordinates. There must be a scrolling position,
	// imagine a website with 2 frames... which one should be scrolled? (sure the one where to mouse
	// currently is, but meh, cef whats more parameters...)
	static void scrollWheel(const QPoint &position, int delta_x, int delta_y);
	
	// Translates qt mouse button keycodes to cef keycodes. Returns -1 on unhandled keycode.
	static int qtToCefMouseButtonType(int qt_button_code);
	
public slots:
	
	// Reloads the current website.
	static void reload();
	
	// Navigate forwards.
	static void goForward();
	
	// Navigate backwards.
	static void goBack();
	
	// Stop loading the current page.
	static void stopLoad();
	
private:
	static void initCefSettings(CefSettings& settings);
};

#endif // !BFP_ENGINE_MODULES_BROWSER_BROWSER_H
