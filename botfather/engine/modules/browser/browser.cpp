#include "browser.h"
#include <iostream>
#include <QApplication>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QElapsedTimer>
#include <QObject>
#include <QStandardPaths>
#include "browser_client.h"
#include "browser_app.h"
#include "browser_settings.h"

#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
#endif

void Browser::init(int argc, char **argv)
{	
	// We don't use the sandbox thus there is no info.
	void* sandbox_info = nullptr;

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
	
	// Calling CefDoMessageLoopWork() a few times before shutting down prevents leaking object references.
	// https://magpcss.org/ceforum/viewtopic.php?f=6&t=14275
	// https://magpcss.org/ceforum/viewtopic.php?f=6&t=13777&start=10
	CefDoMessageLoopWork();
	CefDoMessageLoopWork();
	CefDoMessageLoopWork();
	CefShutdown();
}

void Browser::initCefSettings(CefSettings& settings)
{
	// Works with windows exe files. See https://doc.qt.io/qt-5/qstandardpaths.html#findExecutable
	QString helper_path = QStandardPaths::findExecutable("botfather_helper", { QApplication::applicationDirPath() });
	CefString(&settings.browser_subprocess_path).FromASCII(helper_path.toUtf8());
	
	// Set path to the ressources (cef.pak and/or devtools_resources.pak) directoy.
	CefString(&settings.resources_dir_path) = CefString();

	// Set path to the locales directory.
	CefString(&settings.locales_dir_path) = CefString();
	
	// Defines a cache dir WHICH IS ABSOLUTELY REQUIRED because some site might not work otherwise.
	// IMPORTANT: Requires an absolute path (there is an debug check) in certain situations. Otherwise
	// the program will crash, even thouh relative paths work somethimes.
	QString absolute_cache_path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
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
	
	settings.windowless_rendering_enabled = true;
}

void Browser::resize(const QSize &new_size)
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
    return BrowserClient::instance()->takeScreenshot();
}

void Browser::blockResource(const QString &resource)
{
	Q_ASSERT(BrowserClient::instance());
	BrowserClient::instance()->blockResource(resource);
}

void Browser::replaceResource(const QString &old_resource, const QString &new_resource)
{
	Q_ASSERT(BrowserClient::instance());
	BrowserClient::instance()->replaceResource(old_resource, new_resource);
}

void Browser::unmodifyResource(const QString &resource)
{
	Q_ASSERT(BrowserClient::instance());
	BrowserClient::instance()->unmodifyResource(resource);
}

void Browser::unmodifyResources()
{
	Q_ASSERT(BrowserClient::instance());
	BrowserClient::instance()->unmodifyResources();
}

void Browser::loadUrl(const QString &url)
{
	Q_ASSERT(BrowserClient::instance());
	BrowserClient::instance()->setLoading(true);
	BrowserClient::instance()->getBrowser()->GetMainFrame()->LoadURL(url.toStdString());
}

QUrl Browser::getUrl()
{
	Q_ASSERT(BrowserClient::instance());
	return QUrl(QString::fromStdString(BrowserClient::instance()->getBrowser()->GetMainFrame()->GetURL().ToString()));
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

bool Browser::finishLoading(int timeout_seconds)
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

QSize Browser::getSize()
{
	// TODO: This method of getting the browser size can't be good
	QSettings settings;
	int width = settings.value(browser::options::WIDTH, browser::fallback::WIDTH).toInt();
	int height = settings.value(browser::options::HEIGHT, browser::fallback::HEIGHT).toInt();
	return QSize(width, height);
}

void Browser::executeJavascript(const QString &javascript_code)
{
	Q_ASSERT(BrowserClient::instance());
	BrowserClient::instance()->getBrowser()->GetMainFrame()->ExecuteJavaScript(
		javascript_code.toStdString(),
		BrowserClient::instance()->getBrowser()->GetMainFrame()->GetURL(),
		0
	);
}

void Browser::pressMouse(int button_type, const QPoint &position)
{
	Q_ASSERT(BrowserClient::instance());
	CefBrowserHost::MouseButtonType mbt = CefBrowserHost::MouseButtonType(button_type);
	CefMouseEvent cms;
	cms.x = position.x();
	cms.y = position.y();
	BrowserClient::instance()->getBrowser()->GetHost()->SendMouseClickEvent(cms, mbt, false, 1);
	BrowserClient::instance()->getBrowser()->GetHost()->SendFocusEvent(true);
}

void Browser::releaseMouse(int button_type, const QPoint &position)
{
	Q_ASSERT(BrowserClient::instance());
	CefBrowserHost::MouseButtonType mbt = CefBrowserHost::MouseButtonType(button_type);
	CefMouseEvent cms;
	cms.x = position.x();
	cms.y = position.y();
	BrowserClient::instance()->getBrowser()->GetHost()->SendMouseClickEvent(cms, mbt, true, 1);
}

void Browser::moveMouse(const QPoint &position)
{
	Q_ASSERT(BrowserClient::instance());
	CefMouseEvent cme;
	cme.x = position.x();
	cme.y = position.y();
	QSize size = Browser::getSize();
	bool mouse_leave = position.x() < 0 || position.x() > size.width() || position.y() < 0 || position.y() > size.height();
	BrowserClient::instance()->getBrowser()->GetHost()->SendMouseMoveEvent(cme, mouse_leave);
}

void Browser::scrollWheel(const QPoint &position, int delta_x, int delta_y)
{
	Q_ASSERT(BrowserClient::instance());
	CefMouseEvent cme;
	cme.x = position.x();
	cme.y = position.y();
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
