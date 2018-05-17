#include "browser.h"
#include <iostream>
#include <QApplication>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QElapsedTimer>
#include <QObject>
#include "browser_client.h"
#include "browser_app.h"
#include "browser_settings.h"

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
	
	// Start the Cef Messageloop without having this function blocking
	QTimer *timer = new QTimer();
	QObject::connect(timer, &QTimer::timeout, CefDoMessageLoopWork);
	timer->start(1);
}

void Browser::quit()
{
	// Close all windows so that cef can be shutdown without complaining.
	BrowserClient::instance()->closeAllBrowsers(true);
	CefShutdown();
}

void Browser::initCefSettings(CefSettings& settings)
{
	// Specify the path for the sub-process executable.
#if defined(_WIN32) || defined(_WIN64)
	CefString(&settings.browser_subprocess_path).FromASCII("./botfather_helper.exe");
#else
	CefString(&settings.browser_subprocess_path).FromASCII("./botfather_helper");
#endif
	
	// Set path to the ressources (cef.pak and/or devtools_resources.pak) directoy.
	CefString(&settings.resources_dir_path) = CefString();

	// Set path to the locales directory.
	CefString(&settings.locales_dir_path) = CefString();
	
	// Defines a cache dir WHICH IS ABSOLUTELY REQUIRED because some site might not work otherwise.
	// IMPORTANT: Requires an absolute path (there is an debug check) in certain situations. Otherwise
	// the program will crash, even thouh relative paths work somethimes.
	QString absolute_cache_path = QCoreApplication::applicationDirPath() + "/cache";
	CefString(&settings.cache_path) = CefString(absolute_cache_path.toStdString());

	// Disable extern commandline arguments.
	settings.command_line_args_disabled = true;

	// Make browser process message loop run in a separate thread. Windows only.
	settings.multi_threaded_message_loop = false;

	// Enable logging while debbuging and disable it for production builds.
	settings.log_severity = LOGSEVERITY_DISABLE;
#if defined(QT_DEBUG)
	settings.log_severity = LOGSEVERITY_INFO;
#endif

	// Enable remote debugging on the specified port for debug builds.
#ifndef NDEBUG
	//settings.remote_debugging_port = 8482;
#endif

	// Ignore errors related to invalid SSL certificates. Because we don't
	// have to care about those.
	settings.ignore_certificate_errors = true;

	// Don't use sandbox. Because there are special permissions required and
	// we don't need sandboxed processes at all.
	settings.no_sandbox = true;
}

void Browser::resize(QSize new_size)
{
	Q_ASSERT(BrowserClient::instance());
	// Resize the browser by first setting it's new size and than calling CerBrowserHost::WasReesized to make the browser
	// lookup it's new size by calling CefRenderHandler::GetViewRect.
	QSettings settings;
	settings.setValue(browser::options::WIDTH, new_size.width());
	settings.setValue(browser::options::HEIGHT, new_size.height());
	BrowserClient::instance()->getBrowser()->GetHost()->NotifyMoveOrResizeStarted();
	BrowserClient::instance()->getBrowser()->GetHost()->WasResized();
}

QImage Browser::takeScreenshot()
{
	Q_ASSERT(BrowserClient::instance());
	return BrowserClient::takeScreenshot();
}

void Browser::blockRessource(QString ressource)
{
	Q_ASSERT(BrowserClient::instance());
	BrowserClient::instance()->blockRessource(ressource);
}

void Browser::replaceRessource(QString old_ressource, QString new_ressource)
{
	Q_ASSERT(BrowserClient::instance());
	BrowserClient::instance()->replaceRessource(old_ressource, new_ressource);
}

void Browser::unmodifyRessource(QString ressource)
{
	Q_ASSERT(BrowserClient::instance());
	BrowserClient::instance()->unmodifyRessource(ressource);
}

void Browser::unmodifyRessources()
{
	Q_ASSERT(BrowserClient::instance());
	BrowserClient::instance()->unmodifyRessources();
}

void Browser::loadUrl(QString url)
{
	Q_ASSERT(BrowserClient::instance());
	BrowserClient::instance()->setLoading(true);
	BrowserClient::instance()->getBrowser()->GetMainFrame()->LoadURL(url.toStdString());
}

QString Browser::getUrl()
{
	Q_ASSERT(BrowserClient::instance());
	return QString::fromStdString(BrowserClient::instance()->getBrowser()->GetMainFrame()->GetURL().ToString());
}

void Browser::reloadIgnoringCache()
{
	Q_ASSERT(BrowserClient::instance());
	BrowserClient::instance()->setLoading(true);
	BrowserClient::instance()->getBrowser()->ReloadIgnoreCache();
}

bool Browser::loading()
{
	Q_ASSERT(BrowserClient::instance());
	return BrowserClient::instance()->loading();
}

bool Browser::bideLoading(int timeout_seconds)
{
	Q_ASSERT(BrowserClient::instance());
	
	QElapsedTimer timer;
	timer.start();
	
	while (Browser::loading()) {
		if (timer.hasExpired(timeout_seconds * 1000)) {
			return false;
		}
		QThread::sleep(1);
	}
	
	return true;
}

bool Browser::canGoBack()
{
	Q_ASSERT(BrowserClient::instance());
	return BrowserClient::instance()->getBrowser()->CanGoBack();
}

bool Browser::canGoForward()
{
	Q_ASSERT(BrowserClient::instance());
	return BrowserClient::instance()->getBrowser()->CanGoForward();
}

int Browser::getWidth()
{
	Q_ASSERT(BrowserClient::instance());
	QSettings settings;
	return settings.value(browser::options::WIDTH, browser::fallback::WIDTH).toInt();
}

int Browser::getHeight()
{
	Q_ASSERT(BrowserClient::instance());
	QSettings settings;
	return settings.value(browser::options::HEIGHT, browser::fallback::HEIGHT).toInt();
}

void Browser::executeJavascript(QString javascript_code)
{
	Q_ASSERT(BrowserClient::instance());
	BrowserClient::instance()->getBrowser()->GetMainFrame()->ExecuteJavaScript(
		javascript_code.toStdString(),
		BrowserClient::instance()->getBrowser()->GetMainFrame()->GetURL(),
		0
	);
}

void Browser::pressMouse(int button_type, int x, int y)
{
	Q_ASSERT(BrowserClient::instance());
	CefBrowserHost::MouseButtonType mbt = CefBrowserHost::MouseButtonType(button_type);
	CefMouseEvent cms;
	cms.x = x;
	cms.y = y;
	BrowserClient::instance()->getBrowser()->GetHost()->SendMouseClickEvent(cms, mbt, false, 1);
}

void Browser::releaseMouse(int button_type, int x, int y)
{
	Q_ASSERT(BrowserClient::instance());
	CefBrowserHost::MouseButtonType mbt = CefBrowserHost::MouseButtonType(button_type);
	CefMouseEvent cms;
	cms.x = x;
	cms.y = y;
	BrowserClient::instance()->getBrowser()->GetHost()->SendMouseClickEvent(cms, mbt, true, 1);
}

void Browser::moveMouse(int x, int y)
{
	Q_ASSERT(BrowserClient::instance());
	CefMouseEvent cme;
	cme.x = x;
	cme.y = y;
	bool mouse_leave = x < 0 || x > Browser::getWidth() || y < 0 || y > Browser::getHeight();
	BrowserClient::instance()->getBrowser()->GetHost()->SendMouseMoveEvent(cme, mouse_leave);
}

void Browser::scrollWheel(int x, int y, int delta_x, int delta_y)
{
	Q_ASSERT(BrowserClient::instance());
	CefMouseEvent cme;
	cme.x = x;
	cme.y = y;
	BrowserClient::instance()->getBrowser()->GetHost()->SendMouseWheelEvent(cme, delta_x, delta_y);
}

int Browser::qtToCefMouseButtonType(int qt_button_code)
{
	switch (qt_button_code) {
	case Qt::LeftButton:
		return MBT_LEFT;
	case Qt::MiddleButton:
		return MBT_MIDDLE;
	case Qt::RightButton:
		return MBT_RIGHT;
	default:
		return -1; // Unhandle mouse button type
	}
}

void Browser::reload()
{
	Q_ASSERT(BrowserClient::instance());
	BrowserClient::instance()->setLoading(true);
	BrowserClient::instance()->getBrowser()->Reload();
}

void Browser::goBack()
{
	Q_ASSERT(BrowserClient::instance());
	BrowserClient::instance()->getBrowser()->GoBack();
}

void Browser::goForward()
{
	Q_ASSERT(BrowserClient::instance());
	BrowserClient::instance()->getBrowser()->GoForward();
}

void Browser::stopLoad()
{
	Q_ASSERT(BrowserClient::instance());
	BrowserClient::instance()->getBrowser()->StopLoad();
}