#ifndef BFP__BROWSER__BROWSER_H
#define BFP__BROWSER__BROWSER_H

#include <QString>
#include <include/cef_base.h>

class Browser
{
public:
	// Calling init or quit more than once is not allowed. Also init has to called
	// first and quit before ending the program.
	static void init(int argc, char **argv);
	static void quit();
	
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
	static QString getUrl();
	
	// Reloads the current website.
	static void reload();
	
	// Reloads the current page ignoring any cached data. 
	static void reloadIgnoringCache();
	
	// Stop loading the current page.
	static void stopLoad();
	
	// Returns true if the browser can navigate backwards.
	static bool canGoBack();
	
	// Returns true if the browser can navigate forwards.
	static bool canGoForward();
	
	// Navigate forwards.
	static void goForward();
	
	// Navigate backwards.
	static void goBack();
	
	// Returns the browsers current width.
	static int getWidth();
	
	// Returns the browsers current height.
	static int getHeight();
	
	// Executes the given javascript on the currently loaded website (non-blocking).
	static void executeJavascript(QString javascript_code);
	
	// Sends an click event to the desired coords of the desired type.
	static void clickAt(int type, int x, int y);
		
	// Warps the mouse directly to the desired coordinates.
	static void warpMouseTo(int x, int y);
	
private:
	static void initCefSettings(CefSettings& settings);
};

#endif // !BFP__BROWSER__BROWSER_H