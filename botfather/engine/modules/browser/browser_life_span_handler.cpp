#include "browser_life_span_handler.hpp"
#include "include/wrapper/cef_helpers.h"
#include <Qt>

BrowserLifeSpanHandler::BrowserLifeSpanHandler()
{

}

bool BrowserLifeSpanHandler::isAllowedToClose() const
{
	return m_is_allowed_to_close;
}

void BrowserLifeSpanHandler::setAllowedToClose(bool allowed)
{
	m_is_allowed_to_close = allowed;
}

bool BrowserLifeSpanHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	if (isAllowedToClose())
	{
		// Allow the browser to close
		return false;
	}

	browser->GetMainFrame()->LoadURL("chrome://version/");
	return true;
}

void BrowserLifeSpanHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();
	Q_UNUSED(browser);
}

void BrowserLifeSpanHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();
	Q_UNUSED(browser);
}

bool BrowserLifeSpanHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString &target_url, const CefString &target_frame_name, CefLifeSpanHandler::WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures &popup_features, CefWindowInfo &window_info, CefRefPtr<CefClient> &client, CefBrowserSettings &settings, bool *no_javascript_access)
{
	// Prevent all popups from opening.
	return true;
}
