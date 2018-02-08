#include "browser_app.h"
#include <QDebug>
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "browser_client.h"

void BrowserApp::OnContextInitialized()
{
	CEF_REQUIRE_UI_THREAD();

	// BrowserHandler implements browser-level callbacks.
	CefRefPtr<BrowserClient> handler(new BrowserClient());

	// Specify CEF browser settings here.
	CefBrowserSettings browser_settings;
	//browser_settings.windowless_frame_rate = 30;
	//browser_settings.javascript_open_windows = STATE_DISABLED;
	//browser_settings.javascript_close_windows = STATE_DISABLED;

	// Information used when creating the native window.
	CefWindowInfo window_info;
	window_info.SetAsWindowless(0);

	// Create the browser window.
	CefRefPtr<CefBrowser> browser = CefBrowserHost::CreateBrowserSync(
		window_info,
		handler.get(),
		// "about:blank",
		"",
		browser_settings,
		NULL
	);
	
	// A url (like about:blank) must be loaded first. Otherwise loadString doesnt work.
	/*
	browser->GetMainFrame()->LoadString(
		"<html><body style='background:white;color:black;font-family:sans-serif;display:flex;justify-content:center;align-items:center'>"
		"<p>Hit the &#9655; button to run a bot script.</p>"
		"</body></html>",
		"http://startpage/"
	);
	*/
}

void BrowserApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{
	Q_UNUSED(process_type);

#ifdef Q_OS_LINUX
	command_line->AppendSwitchWithValue("ppapi-flash-path", "libpepflashplayer.so");
	command_line->AppendSwitchWithValue("ppapi-flash-version", "28.0.0.161");
#endif
	
#ifdef Q_OS_WIN
	// Add system flash internally to the command line.
	//command_line->AppendSwitchWithValue("ppapi-flash-path", "pepflashplayer32_23_0_0_207.dll");
	//command_line->AppendSwitchWithValue("ppapi-flash-version", "23.0.0.207");
	command_line->AppendSwitch("enable-system-flash");
#endif
	
	// Loads system plugins like flash in newer CEF versions.
	command_line->AppendSwitch("load-extension");
	
	// Allow cross origion requests, so we may send our swf files when original
	// ones are requested.
	command_line->AppendSwitch("disable-web-security");

	// Enable hardware acceleration. CAUTION! This may break offscreen rendering in certain
	// CEF builds.
	command_line->AppendSwitch("enable-gpu");
	
	//command_line->AppendSwitch("enable-widevine-cdm");
	//command_line->AppendSwitch("disable-extensions");
	//command_line->AppendSwitch("show-paint-rects");
	//command_line->AppendSwitch("disable-gpu");
	//command_line->AppendSwitch("disable-gpu-compositing");
	//command_line->AppendSwitch("disable-accelerated-video-decode");
}

bool BrowserApp::OnBeforeNavigation(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request,
	CefRenderProcessHandler::NavigationType navigation_type,
	bool is_redirect)
{
	return false;
}