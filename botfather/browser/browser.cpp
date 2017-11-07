#include "browser.h"
#include <iostream>
#include <QApplication>
#include <QThread>
#include <QTimer>
#include <QObject>
#include "browser_client.h"
#include "browser_app.h"
#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
#else
	#include <X11/Xlib.h>
#endif

namespace {

#ifdef __linux
int XErrorHandlerImpl(Display* display, XErrorEvent* event)
{
	Q_UNUSED(display);
	LOG(WARNING) \
		<< "X error received: "
		<< "type " << event->type << ", "
		<< "serial " << event->serial << ", "
		<< "error_code " << static_cast<int>(event->error_code) << ", "
		<< "request_code " << static_cast<int>(event->request_code)
		<< ", "
		<< "minor_code " << static_cast<int>(event->minor_code);
	return 0;
}

int XIOErrorHandlerImpl(Display* display)
{
	Q_UNUSED(display);
	return 0;
}
#endif

} // namespace

void Browser::init(int argc, char **argv)
{	
#ifdef __linux
	// Install xlib error handlers so that the application won't be terminated
	// on non-fatal errors.
	XSetErrorHandler(XErrorHandlerImpl);
	XSetIOErrorHandler(XIOErrorHandlerImpl);
#endif
	
	// We don't use the sandbox thus there is no info.
	void* sandbox_info = NULL;

	// Structure for passing command-line arguments. (platform-specific).
#if defined(_WIN32) || defined(_WIN64)
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
	CefMainArgs main_args(hInstance);
#else
	CefMainArgs main_args(argc, argv);
#endif
	
	// Use custom CefApp implementation.
	CefRefPtr<BrowserApp> app(new BrowserApp);

	// Populate this structure to customize CEF behavior.
	CefSettings settings;
	Browser::initCefSettings(settings);

	// Initialize CEF.
	CefInitialize(main_args, settings, app.get(), sandbox_info);
	
	// 
	QTimer *timer = new QTimer();
	QObject::connect(timer, &QTimer::timeout, CefDoMessageLoopWork);
	timer->start(1);
}

void Browser::quit()
{
	// Close all windows so that cef can be shutdown without complaining.
	BrowserClient::GetInstance()->CloseAllBrowsers(true);
	CefShutdown();
}

void Browser::initCefSettings(CefSettings& settings)
{
	// Specify the path for the sub-process executable.
#if defined(_WIN32) || defined(_WIN64)
	CefString(&settings.browser_subprocess_path).FromASCII("botfather_helper.exe");
#else
	CefString(&settings.browser_subprocess_path).FromASCII("./botfather_helper");
#endif
	
	// Set path to the ressources (cef.pak and/or devtools_resources.pak) directoy.
	CefString(&settings.resources_dir_path) = CefString();

	// Set path to the locales directory.
	CefString(&settings.locales_dir_path) = CefString();

	// Disable extern commandline arguments.
	settings.command_line_args_disabled = true;

	// Make browser process message loop run in a separate thread. Windows only.
	settings.multi_threaded_message_loop = false;

	// Enable logging while debbuging and disable it for production builds.
#ifndef NDEBUG
	settings.log_severity = LOGSEVERITY_INFO;
#else
	settings.log_severity = LOGSEVERITY_DISABLE;
#endif

	// Enable remote debugging on the specified port for debug builds.
#ifndef NDEBUG
	settings.remote_debugging_port = 8088;
#endif

	// Ignore errors related to invalid SSL certificates. Because we don't
	// have to care about those.
	settings.ignore_certificate_errors = true;

	// Don't use sandbox. Because there are special permissions required and
	// we don't need sandboxed processes at all.
	settings.no_sandbox = true;
}

void Browser::loadUrl(QString url)
{
	BrowserClient::GetInstance()->GetBrowser()->GetMainFrame()->LoadURL(url.toStdString());
}

QString Browser::getUrl()
{
	return QString::fromStdString(BrowserClient::GetInstance()->GetBrowser()->GetMainFrame()->GetURL().ToString());
}

void Browser::refreshPage(bool ignore_cache)
{
	if (ignore_cache)
		BrowserClient::GetInstance()->GetBrowser()->ReloadIgnoreCache();
	else
		BrowserClient::GetInstance()->GetBrowser()->Reload();
}

void Browser::executeJS(QString javascript)
{
	BrowserClient::GetInstance()->GetBrowser()->GetMainFrame()->ExecuteJavaScript(
		javascript.toStdString(),
		BrowserClient::GetInstance()->GetBrowser()->GetMainFrame()->GetURL(),
		0
	);
}