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
	
	static void loadUrl(QString url);
	static QString getUrl();
	static void refreshPage(bool ignore_cache = false);
	static void executeJS(QString javascript);
	
private:
	static void initCefSettings(CefSettings& settings);
};

#endif // !BFP__BROWSER__BROWSER_H