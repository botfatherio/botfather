#ifndef HELPER_APP_HPP
#define HELPER_APP_HPP

#include "include/cef_app.h"

class HelperApp : public CefApp, public CefRenderProcessHandler {
   public:
    HelperApp();
    CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override {
        return this;
    }
    bool OnProcessMessageReceived(
        CefRefPtr<CefBrowser> browser, CefProcessId source_process,
        CefRefPtr<CefProcessMessage> message) override;

   private:
    IMPLEMENT_REFCOUNTING(HelperApp)
};

#endif  // HELPER_APP_HPP
