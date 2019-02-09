#include "browser_host.h"
#include <QStandardPaths>
#include <QApplication>
#include <QDebug>
#include "include/wrapper/cef_helpers.h"
#include "browser_creator.h"

#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
#endif

BrowserHost *BrowserHost::instance()
{
	static BrowserHost *singleton_instance = new BrowserHost;
	return singleton_instance;
}

bool BrowserHost::init(int argc, char **argv)
{
	// We don't use the sandbox thus there is no info.
	void* sandbox_info = nullptr;

	// Pass command-line arguments
#if defined(_WIN32) || defined(_WIN64)
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(nullptr);
	CefMainArgs main_args(hInstance);
#else
	CefMainArgs main_args(argc, argv);
#endif

	CefRefPtr<BrowserApp> browser_app = new BrowserApp;
	return CefInitialize(main_args, cefSettings(), browser_app.get(), sandbox_info);
}

void BrowserHost::bind(QCoreApplication *app)
{
	CEF_REQUIRE_UI_THREAD();
	Q_ASSERT(m_cef_message_loop_timer == nullptr);

	QObject::connect(app, &QCoreApplication::aboutToQuit, [this]()
	{
		if (m_cef_message_loop_timer)
		{
			m_cef_message_loop_timer->stop();
			m_cef_message_loop_timer->deleteLater();
			m_cef_message_loop_timer = nullptr;
		}
	});
	QObject::connect(app, &QCoreApplication::destroyed, CefShutdown);

	m_cef_message_loop_timer = new QTimer();
	m_cef_message_loop_timer->setInterval(1);
	QObject::connect(m_cef_message_loop_timer, &QTimer::timeout, CefDoMessageLoopWork);
	m_cef_message_loop_timer->start();
}

CefSettings BrowserHost::cefSettings() const
{
	CefSettings cef_settings;

	QString subprocess_path = QStandardPaths::findExecutable("botfather_helper", { QApplication::applicationDirPath() });
	CefString(&cef_settings.browser_subprocess_path).FromASCII(subprocess_path.toUtf8());

	CefString(&cef_settings.resources_dir_path) = CefString(); // cef.pak devtools_resources.pak
	CefString(&cef_settings.locales_dir_path) = CefString();

	// Define a cache dir WHICH IS ABSOLUTELY REQUIRED because some site might not work otherwise.
	// IMPORTANT: Requires an absolute path otherwise the program will crash.
	QString absolute_cache_path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
	CefString(&cef_settings.cache_path) = CefString(absolute_cache_path.toStdString());

	// This option is recommended for use in combination with the DoMessageLoopWork() function
	// in cases where the CEF message loop must be integrated into an existing application message loop
	cef_settings.external_message_pump = true;

	// Windows only option. Turn it off, we have our own message loop
	cef_settings.multi_threaded_message_loop = false;

	// Make our browsers work offscreen and in headless environments.
	cef_settings.windowless_rendering_enabled = true;

	cef_settings.no_sandbox = true;
	cef_settings.command_line_args_disabled = true;

	// Turn off logging in production builds
#if defined(QT_DEBUG)
	cef_settings.log_severity = LOGSEVERITY_INFO;
#else
	cef_settings.log_severity = LOGSEVERITY_DISABLE;
#endif

	// Defaults to black
	cef_settings.background_color = 0xffffffff;

	// Ignore errors related to invalid SSL certificates. Because we don't
	// have to care about those.
	cef_settings.ignore_certificate_errors = true;

	return cef_settings;
}

CefRefPtr<CefBrowser> BrowserHost::createManagedBrowser(const QSize &size, const QString &name)
{
	// TODO: store a reference to the browser before returning it if it's persistent.
	// TODO: check whether the browser were about to create already exists (is persistent and has been created before)
	return BrowserCreator::createBrowserSync(name, size);
}
