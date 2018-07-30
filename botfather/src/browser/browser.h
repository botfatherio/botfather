#ifndef BFP__BROWSER__BROWSER_H
#define BFP__BROWSER__BROWSER_H

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
	static void resize(QSize new_size);
	
	// Returns screenshot of browser as QImage
	static QImage takeScreenshot();
	
	// Adds a ressource to the block list. Blocked ressources are not loaded.
	static void blockRessource(QString ressource);
	
	// Makes the browser load a custom ressource instead of the original.
	static void replaceRessource(QString old_ressource, QString new_ressource);
	
	// Resets block and replace settings for a ressource.
	static void unmodifyRessource(QString ressource);
	
	// Resets block and replace settings for all ressources.
	static void unmodifyRessources();
	
	// Load the given url/website.
	static void loadUrl(QString url);
	
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
	
	// Returns the browsers current width.
	static int getWidth();
	
	// Returns the browsers current height.
	static int getHeight();
	
	// Executes the given javascript on the currently loaded website (non-blocking).
	static void executeJavascript(QString javascript_code);
	
	// Sends a mouse press event to x,y
	static void pressMouse(int button_type, int x, int y);
	
	// Sends a mouse release event to x,y
	static void releaseMouse(int button_type, int x, int y);
	
	// Sends a mouse move event to x,y
	static void moveMouse(int x, int y);

	// Sends a mouse wheel event to the desired xy coordinates. There must be a scrolling position,
	// imagine a website with 2 frames... which one should be scrolled? (sure the one where to mouse
	// currently is, but meh, cef whats more parameters...)
	static void scrollWheel(int x, int y, int delta_x, int delta_y);
	
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

#endif // !BFP__BROWSER__BROWSER_H