#include "browser_manager.hpp"
#include <QStandardPaths>
#include <QApplication>
#include <QThread>
#include <QDebug>
#include "include/wrapper/cef_helpers.h"
#include "browser_creator.hpp"
#include "browser_util.hpp"

#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
#endif

BrowserManager *BrowserManager::instance()
{
	static BrowserManager *singleton_instance = new BrowserManager;
	return singleton_instance;
}

BrowserManager::BrowserManager()
{
	// TODO: Maybe thing about a better parent
	m_model = new BrowserListModel(QCoreApplication::instance());
}

BrowserListModel *BrowserManager::model() const
{
	return m_model;
}

bool BrowserManager::init(int argc, char **argv)
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

void BrowserManager::bind(QCoreApplication *app)
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
		this->quit(); // TODO: eventually make the BrowserManager a QObject
	});

	m_cef_message_loop_timer = new QTimer();
	m_cef_message_loop_timer->setInterval(1);
	QObject::connect(m_cef_message_loop_timer, &QTimer::timeout, CefDoMessageLoopWork);
	m_cef_message_loop_timer->start();
}

void BrowserManager::quit()
{
	deleteBrowsers();
	CefShutdown();
}

CefSettings BrowserManager::cefSettings() const
{
	CefSettings cef_settings;

	QString subprocess_path = QStandardPaths::findExecutable("webhelper", { QApplication::applicationDirPath() });
	CefString(&cef_settings.browser_subprocess_path).FromASCII(subprocess_path.toUtf8());

	CefString(&cef_settings.resources_dir_path) = CefString(); // cef.pak devtools_resources.pak
	CefString(&cef_settings.locales_dir_path) = CefString();

	// Define a cache dir WHICH IS ABSOLUTELY REQUIRED because some site might not work otherwise.
	// IMPORTANT: Requires an absolute path otherwise the program will crash.
	QString absolute_cache_path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
	CefString(&cef_settings.cache_path) = CefString(absolute_cache_path.toStdString());

	// This option is recommended for use in combination with the DoMessageLoopWork() function
	// in cases where the CEF message loop must be integrated into an existing application message loop
	//cef_settings.external_message_pump = true;
	// While this option is the recommended way in our case, it cause the problems described in the following
	// issue: https://bitbucket.org/chromiumembedded/cef/issues/2340/x11-3239-paste-from-system-clipboard-does
	// However, not setting this option to true, did not cause any problems so far.

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

Browser *BrowserManager::createBrowser(const QString &group_name, const QString &browser_id, const QSize &size)
{
	QModelIndexList matches = m_model->match(
		m_model->index(0, 0),
		m_model->BROWSER_UID_ROLE,
		m_model->createUID(group_name, browser_id),
		1,
		Qt::MatchExactly
	);

	if (!matches.isEmpty())
	{
		QVariant variant = m_model->data(matches.first(), m_model->BROWSER_PTR_ROLE);
		Browser *browser = qvariant_cast<Browser*>(variant);
		Q_ASSERT(browser);

		qDebug() << "Returning existing browser" << browser->group() << browser->name();
		return browser;
	}

	CefRefPtr<CefBrowser> cef_browser = BrowserCreator::createBrowserSync(size);
	Browser *browser = new Browser(group_name, browser_id, cef_browser);

	m_model->addBrowser(browser);
	return browser;
}

void BrowserManager::deleteBrowser(Browser *browser)
{
	m_model->removeBrowser(browser);
	CefRefPtr<CefBrowser> cef_browser = browser->cefBrowser();
	delete browser; // Release the Browsers CefRefPtr to the CefBrowser.
	closeCefBrowser(cef_browser);
}

void BrowserManager::deleteBrowsers()
{
	for (Browser *browser : m_model->browsers())
	{
		deleteBrowser(browser);
	}
}

void BrowserManager::closeCefBrowser(CefRefPtr<CefBrowser> browser)
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

	CefRefPtr<CefClient> cef_client = browser->GetHost()->GetClient();
	BrowserClient *browser_client = static_cast<BrowserClient*>(cef_client.get());

	browser_client->setAllowedToClose(true);
	browser->GetHost()->CloseBrowser(true);
}

