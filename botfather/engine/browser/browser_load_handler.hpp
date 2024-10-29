#ifndef BROWSER_LOAD_HANDLER_HPP
#define BROWSER_LOAD_HANDLER_HPP

#include <include/cef_client.h>

class BrowserClient;

class BrowserLoadHandler : public CefLoadHandler {
   public:
    // https://magpcss.org/ceforum/apidocs3/projects/(default)/CefLoadHandler.html
    explicit BrowserLoadHandler(BrowserClient *parent);

    // Called when the browser is done loading a frame.
    void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                   int http_status_code) override;

    // Called when a navigation fails or is canceled.
    void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                     ErrorCode error_code, const CefString &error_rext,
                     const CefString &failed_url) override;

    // Called when the loading state has changed.
    void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool is_loading,
                              bool can_go_back, bool can_go_forward) override;

    // Called after a navigation has been committed and before the browser
    // begins loading contents in the frame.
    void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                     CefLoadHandler::TransitionType transition_type) override;

   private:
    BrowserClient *m_parent;
    IMPLEMENT_REFCOUNTING(BrowserLoadHandler)
};

#endif  // BROWSER_LOAD_HANDLER_HPP
