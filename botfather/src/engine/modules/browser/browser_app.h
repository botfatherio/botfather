#ifndef BFP_ENGINE_MODULES_BROWSER_BROWSER_APP_H
#define BFP_ENGINE_MODULES_BROWSER_BROWSER_APP_H

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
	
private:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(BrowserApp)
};

#endif // !BFP_ENGINE_MODULES_BROWSER_BROWSER_APP_H