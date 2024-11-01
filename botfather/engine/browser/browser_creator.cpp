#include "browser_creator.hpp"

#include <include/wrapper/cef_helpers.h>

#include <QApplication>
#include <QDebug>
#include <QThread>
#include <QTimer>

#include "browser_util.hpp"

CefRefPtr<CefBrowser> BrowserCreator::createBrowserSync(const QSize &size) {
    CefRefPtr<CefBrowser> browser;
    BrowserCreator *browser_creator = new BrowserCreator(size);

    if (CefCurrentlyOn(TID_UI)) {
        qDebug() << "Creating browser from main thread";
        browser_creator->process();
        browser = browser_creator->browser();
        browser_creator->deleteLater();
    } else {
        qDebug() << "Creating browser from non main thread";

        // Browsers can only be created synchronious on the main thread.
        // Run a local event loop until the browser is ready. Then continue the
        // control flow.

        QEventLoop *event_loop = new QEventLoop;
        browser_creator->moveToThread(QApplication::instance()->thread());

        BrowserUtil::runInMainThread([browser_creator, event_loop]() {
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

BrowserCreator::BrowserCreator(const QSize &size, QObject *parent)
    : QObject(parent), m_browser_size(size) {}

CefRefPtr<CefBrowser> BrowserCreator::browser() const { return m_browser; }

void BrowserCreator::process() {
    CEF_REQUIRE_UI_THREAD();

    BrowserClient *browser_client = new BrowserClient(m_browser_size);
    CefRefPtr<BrowserClient> handler(browser_client);
    CefBrowserSettings browser_settings;

    // Allow cross origion requests for resource replacement.
    browser_settings.web_security = STATE_DISABLED;

    CefWindowInfo window_info;
#ifdef OS_WIN
    window_info.SetAsWindowless(nullptr);
#else
    window_info.SetAsWindowless(0);
#endif

    m_browser = CefBrowserHost::CreateBrowserSync(
        window_info, handler.get(), "about:version", browser_settings, nullptr);

    emit finished();
}
