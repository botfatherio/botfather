#include "browser_host.h"
#include <QStandardPaths>
#include <QApplication>
#include <QThread>
#include <QDebug>
#include "include/wrapper/cef_helpers.h"
#include "browser_creator.h"
#include "browser_util.h"

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
	QObject::connect(app, &QCoreApplication::destroyed, [this](){
		this->quit(); // TODO: eventually make the BrowserHost a QObject
	});

	m_cef_message_loop_timer = new QTimer();
	m_cef_message_loop_timer->setInterval(1);
	QObject::connect(m_cef_message_loop_timer, &QTimer::timeout, CefDoMessageLoopWork);
	m_cef_message_loop_timer->start();
}

void BrowserHost::quit()
{
	closeManagedBrowsers();
	CefShutdown();
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

CefRefPtr<CefBrowser> BrowserHost::createManagedBrowser(const QString &group_name, const QSize &size, const QString &id)
{
	if (!id.isEmpty() && m_permanent_browsers.contains(group_name) && m_permanent_browsers[group_name].contains(id))
	{
		qDebug() << "Returning existing persistent browser:" << group_name << id;
		return m_permanent_browsers[group_name][id];
	}

	qDebug() << "Creating new browser:" << group_name << id << size;
	CefRefPtr<CefBrowser> browser = BrowserCreator::createBrowserSync(id, size);

	if (id.isEmpty())
	{
		qDebug() << "Storing new temporary browser:" << group_name << size;
		m_temporary_browsers[group_name].append(browser);
		return browser;
	}

	qDebug() << "Storing new permanent browser:" << group_name << id << size;
	m_permanent_browsers[group_name][id] = browser;
	return browser;
}

void BrowserHost::closeManagedBrowsers()
{
	QList<QString> group_names = m_permanent_browsers.keys() + m_temporary_browsers.keys();
	for (QString group_name : group_names)
	{
		closeManagedBrowsers(group_name);
	}
}

void BrowserHost::closeManagedBrowsers(const QString &group_name)
{
	closeManagedPermanentBrowsers(group_name);
	closeManagedTemporaryBrowsers(group_name);
}

void BrowserHost::closeManagedPermanentBrowsers(const QString &group_name)
{
	if (!m_permanent_browsers.contains(group_name))
	{
		return;
	}

	QHash<QString, CefRefPtr<CefBrowser>> browsers = m_permanent_browsers.take(group_name);
	QHashIterator<QString, CefRefPtr<CefBrowser>> browser_it(browsers);

	while (browser_it.hasNext())
	{
		browser_it.next();
		CefRefPtr<CefBrowser> browser = browsers.take(browser_it.key());

		qDebug() << "Closing permanent browser:" << group_name << browser_it.key();
		closeCefBrowser(browser);
	}
}

void BrowserHost::closeManagedTemporaryBrowsers(const QString &group)
{
	if (!m_temporary_browsers.contains(group))
	{
		return;
	}

	while (!m_temporary_browsers[group].isEmpty())
	{
		CefRefPtr<CefBrowser> browser = m_temporary_browsers[group].takeLast();
		closeCefBrowser(browser);
	}
}

void BrowserHost::closeCefBrowser(CefRefPtr<CefBrowser> browser)
{
	// If this method is called when the application quits, we can't use the
	// qapplications instance to check whether we are in the main thread
	// because its already destroyed.

	if (!CefCurrentlyOn(TID_UI))
	{
		BrowserUtil::runInMainThread([browser](){
			closeCefBrowser(browser);
		});
		return;
	}

	CEF_REQUIRE_UI_THREAD();
	browser->GetHost()->CloseBrowser(true);
}
