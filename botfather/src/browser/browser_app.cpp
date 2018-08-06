#include "browser_app.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonDocument>
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "browser_client.h"
#include "browser_settings.h"

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

	// Allow cross origion requests, so we may send our swf files when original
	// ones are requested.
	browser_settings.web_security = STATE_DISABLED;
	
	// Information used when creating the native window.
	CefWindowInfo window_info;
	window_info.SetAsWindowless(0);

	// Create the browser window.
	CefRefPtr<CefBrowser> browser = CefBrowserHost::CreateBrowserSync(
		window_info,
		handler.get(),
		// "about:blank",
		"about:version",
		browser_settings,
		nullptr
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

static QString parseFlashVersionFromManifest(QString flash_manifest_path)
{
	QFile manifest_file(flash_manifest_path);
	if (!manifest_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return "";
	}
	
	QJsonDocument json = QJsonDocument::fromJson(manifest_file.readAll());
	QString version = json.object().value("version").toString();
	return version;
}

void BrowserApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{
	Q_UNUSED(process_type);
	QSettings settings;

#ifdef Q_OS_LINUX
	QString flash_so(settings.value(browser::options::FLASH_SO).toString());
	QString flash_manifest(settings.value(browser::options::FLASH_MANIFEST).toString());
	
	QFileInfo flash_so_info(flash_so);
	QFileInfo flash_manifest_info(flash_manifest);
	
	bool flash_so_exists = flash_so_info.exists() && flash_so_info.isFile();
	bool flash_manifest_exists = flash_manifest_info.exists() && flash_manifest_info.isFile();

	if (flash_so_exists && flash_manifest_exists) {
		
		QString flash_version = parseFlashVersionFromManifest(flash_manifest);
		if (!flash_version.isEmpty()) {
			command_line->AppendSwitchWithValue("ppapi-flash-path", flash_so.toStdString());
			command_line->AppendSwitchWithValue("ppapi-flash-version", flash_version.toStdString());
		}
	}
#else
	if (settings.value(browser::options::USE_SYSTEM_FLASH, browser::fallback::USE_SYSTEM_FLASH).toBool()) {
		command_line->AppendSwitch("enable-system-flash");
	}
#endif

	// Loads system plugins like flash in newer CEF versions.
	command_line->AppendSwitch("load-extension");
	
	// Enable hardware acceleration. CAUTION! This may break offscreen rendering in certain
	// CEF builds.
	//command_line->AppendSwitch("enable-gpu");
	
	// Use software rendering and compositing (disable GPU) for increased FPS
	// and decreased CPU usage. This will also disable WebGL so remove these
	// switches if you need that capability.
	// See https://bitbucket.org/chromiumembedded/cef/issues/1257 for details.
	command_line->AppendSwitch("disable-gpu");
	command_line->AppendSwitch("disable-gpu-compositing");
	
	// Synchronize the frame rate between all processes. This results in
	// decreased CPU usage by avoiding the generation of extra frames that
	// would otherwise be discarded. The frame rate can be set at browser
	// creation time via CefBrowserSettings.windowless_frame_rate or changed
	// dynamically using CefBrowserHost::SetWindowlessFrameRate. In cefclient
	// it can be set via the command-line using `--off-screen-frame-rate=XX`.
	// See https://bitbucket.org/chromiumembedded/cef/issues/1368 for details.
	command_line->AppendSwitch("enable-begin-frame-scheduling");
	
	// Necesarry to view DRM protected content
	command_line->AppendSwitch("enable-widevine-cdm");
	
	// On some CEF versions only commandline args disable sandbox properly
	command_line->AppendSwitch("no-sandbox");
	command_line->AppendSwitch("disable-setuid-sandbox");
	
	//command_line->AppendSwitch("show-paint-rects");
	//command_line->AppendSwitch("disable-accelerated-video-decode");
}