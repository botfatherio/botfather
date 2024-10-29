#include "browser_load_handler.hpp"

#include <QString>

#include "browser_client.hpp"
#include "include/wrapper/cef_helpers.h"

BrowserLoadHandler::BrowserLoadHandler(BrowserClient *parent)
    : m_parent(parent) {}

void BrowserLoadHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   int http_status_code) {
    emit m_parent->finishedLoadingUrl(
        QString::fromStdString(browser->GetMainFrame()->GetURL()));
}

void BrowserLoadHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     ErrorCode error_code,
                                     const CefString &error_text,
                                     const CefString &failed_url) {}

void BrowserLoadHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                              bool is_loading, bool can_go_back,
                                              bool can_go_forward) {
    emit m_parent->loadingStateChanged(is_loading, can_go_back, can_go_forward);
}

void BrowserLoadHandler::OnLoadStart(
    CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
    CefLoadHandler::TransitionType transition_type) {}
