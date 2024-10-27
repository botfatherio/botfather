#ifndef BROWSER_LIFE_SPAN_HANDLER_HPP
#define BROWSER_LIFE_SPAN_HANDLER_HPP

#include <include/cef_client.h>

class BrowserLifeSpanHandler : public CefLifeSpanHandler {
   public:
    // https://magpcss.org/ceforum/apidocs3/projects/(default)/CefLifeSpanHandler.html
    BrowserLifeSpanHandler();

    // When a website closes itself, its underlying CefBrowser usually would get
    // destroyed. Unless we explicitly allowed to browser to close itself we
    // prevent it.
    bool isAllowedToClose() const;
    void setAllowedToClose(bool allowed);

    // Called when a browser has recieved a request to close.
    bool DoClose(CefRefPtr<CefBrowser> browser) override;

    // Called after a new browser is created.
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;

    // Called just before a browser is destroyed.
    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

    // Called on the UI thread before a new popup browser is created.
    bool OnBeforePopup(
        CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
        const CefString& target_url, const CefString& target_frame_name,
        CefLifeSpanHandler::WindowOpenDisposition target_disposition,
        bool user_gesture, const CefPopupFeatures& popup_features,
        CefWindowInfo& window_info, CefRefPtr<CefClient>& client,
        CefBrowserSettings& settings, bool* no_javascript_access) override;

   private:
    bool m_is_allowed_to_close = false;
    IMPLEMENT_REFCOUNTING(BrowserLifeSpanHandler)
};

#endif  // BROWSER_LIFE_SPAN_HANDLER_HPP
