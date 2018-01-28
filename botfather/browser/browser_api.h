#ifndef BROWSER_API_H
#define BROWSER_API_H

#include <QObject>
#include <QJSEngine>
#include <QString>
#include <QSettings>

class BrowserAPI : public QObject
{
	Q_OBJECT
public:
	explicit BrowserAPI(QJSEngine* engine_p);
	static void enable(QJSEngine* engine_p);
	
	// Adds a ressource to the block list. Blocked ressources are not loaded.
	Q_INVOKABLE void blockRessource(QString ressource);
	
	// Makes the browser load a custom ressource instead of the original.
	Q_INVOKABLE void replaceRessource(QString original_ressource, QString custom_ressource);
	
	// Resets block and replace settings for a ressource.
	Q_INVOKABLE void unmodifyRessource(QString ressource);
	
	// Load the given url/website.
	Q_INVOKABLE void loadUrl(QString url);
	
	// Returns the currently loaded url.
	Q_INVOKABLE QString getUrl();
	
	// Reloads the current website.
	Q_INVOKABLE void reload();
	
	// Reloads the current page ignoring any cached data. 
	Q_INVOKABLE void reloadIgnoreCache();
	
	// Stop loading the current page.
	Q_INVOKABLE void stopLoad();
	
	// Returns true if the browser can navigate backwards.
	Q_INVOKABLE bool canGoBack();
	
	// Returns true if the browser can navigate forwards.
	Q_INVOKABLE bool canGoForward();
	
	// Navigate forwards.
	Q_INVOKABLE void goForward();
	
	// Navigate backwards.
	Q_INVOKABLE void goBack();
	
	// Returns the browsers current width.
	Q_INVOKABLE int getWidth();
	
	// Returns the browsers current height.
	Q_INVOKABLE int getHeight();
	
	// Executes the given javascript on the currently loaded website (non-blocking).
	Q_INVOKABLE void executeJavascript(QString javascript_code);
	
private:
	QJSEngine* m_engine_p;
	QSettings m_settings;
};

#endif // BROWSER_API_H