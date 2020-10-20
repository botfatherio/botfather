#ifndef BROWSERAPP_H
#define BROWSERAPP_H

#include "include/cef_app.h"

class BrowserApp : public CefApp, public CefBrowserProcessHandler {
   public:
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE {
        return this;
    }

    // Provides an opportunity to view/modify command-line arguments before they
    // are processed by CEF
    void OnBeforeCommandLineProcessing(
        const CefString& process_type,
        CefRefPtr<CefCommandLine> command_line) OVERRIDE;

   private:
    IMPLEMENT_REFCOUNTING(BrowserApp)
};

#endif  // BROWSERAPP_H
