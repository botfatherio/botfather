#ifndef BROWSER_REQUEST_HANDLER_HPP
#define BROWSER_REQUEST_HANDLER_HPP

#include "include/cef_client.h"
#include <QVector>
#include <QString>

class BrowserClient;

class BrowserRequestHandler : public CefRequestHandler
{
public:
	// https://magpcss.org/ceforum/apidocs3/projects/(default)/CefRequestHandler.html
	explicit BrowserRequestHandler(BrowserClient *parent);

	// Makes the resource no longe been loaded by the browser.
	void blockResource(const QString &resource_pattern);

	// Makes the browser replace any resource matching the 'resource_pattern' with 'replacement_url'.
	void replaceResource(const QString &resource_pattern, const QString &replacement_url);

	// Removes any replace or block rule from a given resource.
	void unmodifyResource(const QString &resource_pattern);

	// Removes all replace and block rules for all resources.
	void unmodifyResources();

    // Override the user agent
    void setUserAgent(const QString &user_agent);

    // Reset the user agent
    void resetUserAgent();

	// Called on the IO thread before a resource request is loaded.
	CefRequestHandler::ReturnValue OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback) override;

	// Called on the browser process UI thread when a plugin has crashed.
	void OnPluginCrashed(CefRefPtr<CefBrowser> browser, const CefString &plugin_path) override;

	// Called on the browser process UI thread when the render process terminates unexpectedly.
	void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, CefRequestHandler::TerminationStatus status) override;

private:
	BrowserClient *m_parent;

	// Contains all redirected urls. The first item of a pair is a regex pattern matching
	// the original url.
	// The second item is either empty, meaning the url should not be loaded (is blocked),
	// or there is a second url provided, meaning the second url should be loaded instead.
	QVector<QPair<QString, QString>> m_modified_resources;

    QString m_user_agent_override;

	IMPLEMENT_REFCOUNTING(BrowserRequestHandler)
};

#endif // BROWSER_REQUEST_HANDLER_HPP
