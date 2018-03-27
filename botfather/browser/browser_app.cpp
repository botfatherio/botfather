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
#include "../shared/settings.h"

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
		"about:version",
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
	QString flash_so(settings.value(options::browser::FLASH_SO).toString());
	QString flash_manifest(settings.value(options::browser::FLASH_MANIFEST).toString());
	
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
	if (settings.value(options::browser::USE_SYSTEM_FLASH, fallback::browser::USE_SYSTEM_FLASH).toBool()) {
		command_line->AppendSwitch("enable-system-flash");
	}
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