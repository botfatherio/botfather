#include "browser_creator.h"
#include <QApplication>
#include <QTimer>
#include <QThread>
#include <QDebug>
#include "include/wrapper/cef_helpers.h"
#include "browser_util.h"

CefRefPtr<CefBrowser> BrowserCreator::createBrowserSync(const QString &name, const QSize &size)
{
	CefRefPtr<CefBrowser> browser;
	BrowserCreator *browser_creator = new BrowserCreator(name, size);

	if (CefCurrentlyOn(TID_UI))
	{
		qDebug() << "Creating browser from main thread";
		browser_creator->process();
		browser = browser_creator->browser();
		browser_creator->deleteLater();
	}
	else
	{
		qDebug() << "Creating browser from non main thread";

		// Browsers can only be created synchronious on the main thread.
		// Run a local event loop until the browser is ready. Then continue the control flow.

		QEventLoop *event_loop = new QEventLoop;
		browser_creator->moveToThread(QApplication::instance()->thread());

		BrowserUtil::runInMainThread([browser_creator, event_loop](){
			browser_creator->process();
			event_loop->quit();
		});

		event_loop->exec();
		browser = browser_creator->browser();

		browser_creator->deleteLater();
		event_loop->deleteLater();
	}

	return browser;
}

BrowserCreator::BrowserCreator(const QString &name, const QSize &size, QObject *parent)
	: QObject(parent)
	, m_browser_size(size)
	, m_browser_name(name)
{

}

CefRefPtr<CefBrowser> BrowserCreator::browser() const
{
	return m_browser;
}

void BrowserCreator::process()
{
	CEF_REQUIRE_UI_THREAD();

	BrowserClient *browser_client = new BrowserClient(m_browser_name, m_browser_size);
	CefRefPtr<BrowserClient> handler(browser_client);
	CefBrowserSettings browser_settings;

	// Allow cross origion requests for resource replacement.
	browser_settings.web_security = STATE_DISABLED;

	CefWindowInfo window_info;
	window_info.SetAsWindowless(0);

	m_browser = CefBrowserHost::CreateBrowserSync(
		window_info,
		handler.get(),
		"about:version",
		browser_settings,
		nullptr
	);

	emit finished();
}
