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

CefRefPtr<CefBrowser> BrowserHost::createManagedBrowser(const QString &group, const QString &name, const QSize &size)
{
	if (!name.isEmpty() && m_map_of_grouped_named_browsers.contains(group) && m_map_of_grouped_named_browsers[group].contains(name))
	{
		qDebug() << "Returning aleady existing browser:" << group << name;
		return m_map_of_grouped_named_browsers[group][name];
	}

	CefRefPtr<CefBrowser> browser = BrowserCreator::createBrowserSync(name, size);

	// Both named and unnamed browsers and their group must be remembered.
	// That way they can be closed and freed when they are no longer needed.
	// In our case a browsers group is the bot who created the browser.

	if (name.isEmpty())
	{
		// Unnamed browsers an empty name and can't be stored in the map of
		// named browsers to avoid name/key collisions. Random names won't
		// work as they limit the script-writers choice of name.
		// Prefixing doesn't work either as every browser would then be
		// considered named/persistent.
		qDebug() << "Creating new unnamed browser in:" << group;
		m_list_of_grouped_unnamed_browsers[group].append(browser);
		return browser;
	}

	qDebug() << "Creating new named browser:" << name << group;
	m_map_of_grouped_named_browsers[group][name] = browser;
	return browser;
}

void BrowserHost::closeManagedBrowsers()
{
	QHashIterator<QString, QHash<QString, CefRefPtr<CefBrowser>>> groups_it(m_map_of_grouped_named_browsers);
	while (groups_it.hasNext())
	{
		groups_it.next();
		closeManagedBrowsers(groups_it.key());
	}
}

void BrowserHost::closeManagedBrowsers(const QString &group)
{
	closeManagedNamedBrowsers(group);
	closeManagedUnnamedBrowsers(group);
}

void BrowserHost::closeManagedNamedBrowsers(const QString &group)
{
	if (!m_map_of_grouped_named_browsers.contains(group))
	{
		return;
	}

	QHash<QString, CefRefPtr<CefBrowser>> group_map = m_map_of_grouped_named_browsers.take(group);
	QHashIterator<QString, CefRefPtr<CefBrowser>> browser_it(group_map);

	while (browser_it.hasNext())
	{
		browser_it.next();
		CefRefPtr<CefBrowser> browser = group_map.take(browser_it.key());

		qDebug() << "Closing named browser:" << group << browser_it.key();
		closeCefBrowser(browser);
	}
}

void BrowserHost::closeManagedUnnamedBrowsers(const QString &group)
{
	if (!m_list_of_grouped_unnamed_browsers.contains(group))
	{
		return;
	}

	while (!m_list_of_grouped_unnamed_browsers[group].isEmpty())
	{
		CefRefPtr<CefBrowser> browser = m_list_of_grouped_unnamed_browsers[group].takeLast();
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
