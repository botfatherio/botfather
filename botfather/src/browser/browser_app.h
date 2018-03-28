#ifndef BFP__BROWSER__BROWSER_APP_H
#define BFP__BROWSER__BROWSER_APP_H

#include "include/cef_app.h"

class BrowserApp :
	public CefApp,
	public CefBrowserProcessHandler,
	public CefRenderProcessHandler
{
public:
	// Return the handler for functionality specific to the browser process.
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE { return this; }

	// Return the handler for functionality specific to the render process.
	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE { return this; }

	// Provides an opportunity to view and/or modify command-line arguments before processing by CEF and Chromium.
	virtual void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) OVERRIDE;

	// Called on the browser process UI thread immediately after the CEF context has been initialized.
	virtual void OnContextInitialized() OVERRIDE;

	// |CefRenderProcessHandler|. Called before browser navigation. Return true
	// to cancel the navigation or false to allow the navigation to proceed.
	// The |request| object cannot be modified in this callback. 
	virtual bool OnBeforeNavigation(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		CefRenderProcessHandler::NavigationType navigation_type,
		bool is_redirect) OVERRIDE;
	
private:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(BrowserApp);
};

#endif // !BFP__BROWSER__BROWSER_APP_H