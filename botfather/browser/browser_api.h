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
	Q_INVOKABLE static void blockRessource(QString ressource);
	
	// Makes the browser load a custom ressource instead of the original.
	Q_INVOKABLE static void replaceRessource(QString old_ressource, QString new_ressource);
	
	// Resets block and replace settings for a ressource.
	Q_INVOKABLE static void unmodifyRessource(QString ressource);
	
	// Resets block and replace settings for all ressources.
	Q_INVOKABLE static void unmodifyRessources();
	
	// Load the given url/website.
	Q_INVOKABLE static void loadUrl(QString url);
	
	// Returns the currently loaded url.
	Q_INVOKABLE static QString getUrl();
	
	// Reloads the current website.
	Q_INVOKABLE static void reload();
	
	// Reloads the current page ignoring any cached data. 
	Q_INVOKABLE static void reloadIgnoreCache();
	
	// Stop loading the current page.
	Q_INVOKABLE static void stopLoad();
	
	// Returns true if the browser can navigate backwards.
	Q_INVOKABLE static bool canGoBack();
	
	// Returns true if the browser can navigate forwards.
	Q_INVOKABLE static bool canGoForward();
	
	// Navigate forwards.
	Q_INVOKABLE static void goForward();
	
	// Navigate backwards.
	Q_INVOKABLE static void goBack();
	
	// Returns the browsers current width.
	Q_INVOKABLE int getWidth();
	
	// Returns the browsers current height.
	Q_INVOKABLE int getHeight();
	
	// Executes the given javascript on the currently loaded website (non-blocking).
	Q_INVOKABLE static void executeJavascript(QString javascript_code);
	
private:
	QJSEngine* m_engine_p;
	QSettings m_settings;
};

#endif // BROWSER_API_H