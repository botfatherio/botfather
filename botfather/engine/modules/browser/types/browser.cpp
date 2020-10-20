#include "browser.hpp"

#include <QDebug>
#include <QElapsedTimer>
#include <QThread>
#include <QTimer>
#include <QUuid>
#include <functional>

#include "../../common/bf_key_mapper.hpp"
#include "../../common/bf_keymap.hpp"
#include "../adapters/cef_key_event_adapter.hpp"
#include "../browser_settings.hpp"
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"

Browser::Browser(const QString &group, const QString &id,
                 CefRefPtr<CefBrowser> cef_browser)
    : m_group(group), m_name(id), m_cef_browser(cef_browser) {
    CefRefPtr<CefClient> cef_client = m_cef_browser->GetHost()->GetClient();
    m_browser_client = static_cast<BrowserClient *>(cef_client.get());
    Q_ASSERT(m_browser_client);
}

CefRefPtr<CefBrowser> Browser::cefBrowser() const { return m_cef_browser; }

BrowserClient *Browser::client() const { return m_browser_client; }

QString Browser::group() const { return m_group; }

QString Browser::name() const { return m_name; }

QRect Browser::rect() const { return QRect(QPoint(0, 0), size()); }

QUrl Browser::url() const {
    return QString::fromStdString(m_cef_browser->GetMainFrame()->GetURL());
}

QSize Browser::size() const { return m_browser_client->size(); }

void Browser::resize(const QSize &size) {
    Q_ASSERT(size.width() >= MIN_WIDTH);
    Q_ASSERT(size.height() >= MIN_HEIGHT);
    m_browser_client->setSize(size);
}

QImage Browser::takeScreenshot() { return m_browser_client->takeScreenshot(); }

bool Browser::isLoading() const { return m_cef_browser->IsLoading(); }

bool Browser::finishLoading(int timeout_in_seconds) {
    QElapsedTimer timer;
    timer.start();

    while (isLoading()) {
        if (timer.hasExpired(timeout_in_seconds * 1000)) {
            return false;
        }
        QThread::sleep(1);
    }

    return true;
}

bool Browser::canGoBack() const { return m_cef_browser->CanGoBack(); }

bool Browser::canGoForward() const { return m_cef_browser->CanGoForward(); }

void Browser::blockResource(const QString &resource) {
    m_browser_client->blockResource(resource);
}

void Browser::replaceResource(const QString &resource_pattern,
                              const QString &replacement_url) {
    m_browser_client->replaceResource(resource_pattern, replacement_url);
}

void Browser::unmodifyResource(const QString &resource_pattern) {
    m_browser_client->unmodifyResource(resource_pattern);
}

void Browser::unmodifyResources() { m_browser_client->unmodifyResources(); }

void Browser::setUserAgent(const QString &user_agent) {
    m_browser_client->setUserAgent(user_agent);
}

void Browser::resetUserAgent() { m_browser_client->resetUserAgent(); }

void Browser::loadUrl(const QString &url) {
    m_cef_browser->GetMainFrame()->LoadURL(url.toStdString());
}

void Browser::beOnUrl(const QString &the_url) {
    if (QUrl(the_url) != url()) {
        loadUrl(the_url);
    }
}

void Browser::goHome() { loadUrl("about:version"); }

void Browser::reload() { m_cef_browser->Reload(); }

void Browser::reloadIgnoringCache() { m_cef_browser->ReloadIgnoreCache(); }

void Browser::stopLoading() { m_cef_browser->StopLoad(); }

void Browser::goBack() { m_cef_browser->GoBack(); }

void Browser::goForward() { m_cef_browser->GoForward(); }

void Browser::executeJavascript(const QString &javascript_code) {
    m_cef_browser->GetMainFrame()->ExecuteJavaScript(
        javascript_code.toStdString(), m_cef_browser->GetMainFrame()->GetURL(),
        0);
}

static void send_eval_javascript_message(CefRefPtr<CefBrowser> cef_browser,
                                         const QString &script_name,
                                         const QString &javascript_code,
                                         const QString &result_id) {
    CefRefPtr<CefProcessMessage> msg =
        CefProcessMessage::Create("eval_javascript");
    CefRefPtr<CefListValue> args = msg->GetArgumentList();
    args->SetString(0, CefString(result_id.toStdString()));
    args->SetString(1, CefString(javascript_code.toStdString()));
    args->SetString(2, CefString(script_name.toStdString()));
    cef_browser->SendProcessMessage(PID_RENDERER, msg);
}

bool Browser::evaluateJavascript(const QString &script_name,
                                 const QString &javascript_code,
                                 int timeout_in_ms, QCborValue &result,
                                 QVariantMap &exception, bool &timed_out) {
    bool success = false;

    // Generate a UUID to identify the result that belongs to this call to
    // Browser::evaluateJavascript. Evaluating a script with a short
    // |timeout_in_ms| followed by evaluating another script could result in the
    // second script receiving the result of the first evaluation. Giving each
    // result a UUID allows us to ignore results which do not belong to this
    // method call.
    QString result_id = QUuid::createUuid().toString();

    QEventLoop loop;
    QTimer timer;

    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);

    QSharedPointer<QMetaObject::Connection> conn(new QMetaObject::Connection);

    auto slot = [this, result_id, conn, &timer, &success, &result, &exception](
                    const QString &id, const bool &s, const QCborValue &r,
                    const QVariantMap &e) {
        // Ignore results which do not correspond to this call to
        // evaluateJavascript
        if (result_id != id) return;

        // Disconnect this lambda slot from the evalJavascriptResultReady
        // signal, to prevent more signals from arriving
        // QObject::disconnect(*conn);
        this->disconnect(*conn);  // FIXME: UNTESTED UNDER WINDOWS

        // Stop the timer to prevent the call from timing out
        QMetaObject::invokeMethod(&timer, "stop", Qt::QueuedConnection);

        success = s;
        result = r;
        exception = e;
    };

    *conn = connect(m_browser_client, &BrowserClient::evalJavascriptResultReady,
                    slot);

    // Calling QEventLoop::quit from within the lambda will occasionally cause
    // crashes. Connecting the signals in the given order works perfectly.
    connect(m_browser_client, &BrowserClient::evalJavascriptResultReady, &loop,
            &QEventLoop::quit);

    timer.start(timeout_in_ms);

    // CefBrowser::SendProcessMessage must be send from the main thread of the
    // browser process. TID_UI thread is the main thread in the browser process.
    CefPostTask(TID_UI, base::Bind(&send_eval_javascript_message, m_cef_browser,
                                   script_name, javascript_code, result_id));

    // Run the event loop (blocking)
    loop.exec();

    timed_out = !timer.isActive();
    return success;
}

static int convertToCefMouseButtonType(int qt_mouse_button) {
    switch (qt_mouse_button) {
        case Qt::LeftButton:
            return MBT_LEFT;
        case Qt::MiddleButton:
            return MBT_MIDDLE;
        case Qt::RightButton:
            return MBT_RIGHT;
        default:
            return -1;  // Unhandled mouse button type
    }
}

void Browser::pressMouse(const QPoint &position, int qt_mouse_button) {
    holdMouse(position, qt_mouse_button);
    releaseMouse(position, qt_mouse_button);
}

void Browser::holdMouse(const QPoint &position, int qt_mouse_button) {
    int button_code = convertToCefMouseButtonType(qt_mouse_button);
    if (button_code == -1) return;

    CefMouseEvent event;
    event.x = position.x();
    event.y = position.y();

    m_cef_browser->GetHost()->SendFocusEvent(true);
    m_cef_browser->GetHost()->SendMouseClickEvent(
        event, CefBrowserHost::MouseButtonType(button_code), false, 1);
}

void Browser::releaseMouse(const QPoint &position, int qt_mouse_button) {
    int button_code = convertToCefMouseButtonType(qt_mouse_button);
    if (button_code == -1) return;

    CefMouseEvent event;
    event.x = position.x();
    event.y = position.y();

    m_cef_browser->GetHost()->SendMouseClickEvent(
        event, CefBrowserHost::MouseButtonType(button_code), true, 1);
}

void Browser::moveMouse(const QPoint &position) {
    CefMouseEvent event;
    event.x = position.x();
    event.y = position.y();
    bool mouse_leaves = !rect().contains(position);
    m_cef_browser->GetHost()->SendMouseMoveEvent(event, mouse_leaves);
}

void Browser::scrollWheel(const QPoint &position, const QPoint &delta) {
    CefMouseEvent event;
    event.x = position.x();
    event.y = position.y();
    m_cef_browser->GetHost()->SendMouseWheelEvent(event, delta.x(), delta.y());
}

bool Browser::canPressKey(const QString &bf_keycode) const {
    // CefKeyEvents can be easily created from single characters. Though they
    // might get interpreted wrong.
    return bf_keycode.length() == 1 || BF_KEYMAP.contains(bf_keycode);
}

bool Browser::canEnterText(const QString &text) const {
    for (int i = 0; i < text.length(); ++i) {
        if (!canPressKey(text.at(i))) {
            return false;
        }
    }
    return true;
}

void Browser::pressKey(const QString &bf_keycode) {
    holdKey(bf_keycode);
    releaseKey(bf_keycode);
}

void Browser::holdKey(const QString &bf_keycode) {
    Qt::KeyboardModifier modifier =
        BFKeyMapper::mapBFKeycodeToQtKeyboardModifier(bf_keycode);
    if (!m_unreleased_keyboard_modifiers.testFlag(modifier)) {
        m_unreleased_keyboard_modifiers =
            m_unreleased_keyboard_modifiers.setFlag(modifier, true);
    }

    CefKeyEventAdapter key_event(bf_keycode, m_unreleased_keyboard_modifiers);
    holdKey(key_event);
}

void Browser::releaseKey(const QString &bf_keycode) {
    Qt::KeyboardModifier modifier =
        BFKeyMapper::mapBFKeycodeToQtKeyboardModifier(bf_keycode);
    if (m_unreleased_keyboard_modifiers.testFlag(modifier)) {
        m_unreleased_keyboard_modifiers =
            m_unreleased_keyboard_modifiers.setFlag(modifier, false);
    }

    CefKeyEventAdapter key_event(bf_keycode, m_unreleased_keyboard_modifiers);
    releaseKey(key_event);
}

void Browser::holdKey(const CefKeyEvent &event) {
    // Analyzing the cefclient osr windows example generates the following
    // CefKeyEvents when a "(" is entered:
    //
    // - windows_key_code: 16, type: 0, modifiers: 1026 <-> Shift RawKeyDown
    // Event, 16 being the VK of the shift key.
    // - windows_key_code: 56, type: 0, modifiers: 2    <-> 8-Key RawKeyDown
    // Event, 56 being the VK of the 8 key.
    // - windows_key_code: 40, type: 3, modifiers: 514  <-> '(' Char Event, 40
    // being the unicode value of '('.
    // - windows_key_code: 56, type: 2, modifiers: 2    <-> 8-key KeyUp Event
    // - windows_key_code: 16, type: 2, modifiers: 0    <-> Shift KeyUp Event
    //
    // The first 4 events have shift modifiers. The 5th event releases the shift
    // key, thus there is no shift modifier anymore. The native_key_code was
    // either uninizialized or some high number. As the native key code is
    // specific to the keyboard used, setting it does not make a difference in
    // our scenario.
    //
    // The Windows Keyboard Input Documentation describes a similar behaviour:
    // https://docs.microsoft.com/en-us/windows/desktop/learnwin32/keyboard-input
    //
    // The CefKeyEvent character property was always set to '\0' under windows.
    // However we set it to the unicode value of the entered character (if any)
    // and use it to generate an CHAR event as described below.
    //
    // Qt however does only generate 2 QKeyEvents in the same situation. One
    // KeyPress and a KeyRelease. To make it work we generate a CHAR event from
    // the KeyPress event, by overriding the windows_key_code (and the
    // native_key_code) with the characters unicode value and adjusting the
    // type.

    CefKeyEvent event_copy(event);

    event_copy.type = KEYEVENT_RAWKEYDOWN;
    m_cef_browser->GetHost()->SendKeyEvent(event_copy);

    event_copy.type = KEYEVENT_CHAR;
    event_copy.native_key_code = event_copy.character;
    event_copy.windows_key_code = event_copy.character;
    m_cef_browser->GetHost()->SendKeyEvent(event_copy);
}

void Browser::releaseKey(const CefKeyEvent &event) {
    CefKeyEvent event_copy(event);
    event_copy.type = KEYEVENT_KEYUP;
    m_cef_browser->GetHost()->SendKeyEvent(event_copy);
}

void Browser::enterText(const QString &text) {
    for (int i = 0; i < text.length(); ++i) {
        pressKey(text.at(i));
    }
}

void Browser::holdKey(const QKeyEvent *event) {
    if (event->matches(QKeySequence::Copy)) {
        m_cef_browser->GetFocusedFrame()->Copy();
    } else if (event->matches(QKeySequence::Cut)) {
        m_cef_browser->GetFocusedFrame()->Cut();
    } else if (event->matches(QKeySequence::Paste)) {
        m_cef_browser->GetFocusedFrame()->Paste();
    } else if (event->matches(QKeySequence::Undo)) {
        m_cef_browser->GetFocusedFrame()->Undo();
    } else if (event->matches(QKeySequence::Redo)) {
        m_cef_browser->GetFocusedFrame()->Redo();
    } else if (event->matches(QKeySequence::SelectAll)) {
        m_cef_browser->GetFocusedFrame()->SelectAll();
    } else {
        CefKeyEventAdapter key_event(event);
        holdKey(key_event);
    }
}

void Browser::releaseKey(const QKeyEvent *event) {
    CefKeyEventAdapter key_event(event);
    releaseKey(key_event);
}
