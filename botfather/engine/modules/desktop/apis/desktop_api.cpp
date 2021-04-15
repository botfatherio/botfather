#include "desktop_api.hpp"

#include <QDebug>
#include <QPoint>

#include "../../vision/apis/vision_api.hpp"
#include "../../vision/prototypes/image_prototype.hpp"
#include "../../vision/types/match.hpp"
#include "../../vision/vision.hpp"

DesktopAPI::DesktopAPI(Engine *bot, QObject *parent)
    : AbstractAPI(bot, parent) {
    desktop = new Desktop(this);
}

QImage DesktopAPI::takeScreenshot() {
    QImage qimage = desktop->takeScreenshot();
    engine()->reportAdditionalMemoryCost(ImageSizeInBytes(qimage));
    return qimage;
}

QSize DesktopAPI::getSize() { return desktop->getSize(); }

QRect DesktopAPI::getRect() { return QRect(QPoint(0, 0), desktop->getSize()); }

void DesktopAPI::leftClick(const QPoint &position) {
    desktop->warpCursor(position);
    desktop->pressMouse(Desktop::MouseButtons::left);
}

void DesktopAPI::middleClick(const QPoint &position) {
    desktop->warpCursor(position);
    desktop->pressMouse(Desktop::MouseButtons::middle);
}

void DesktopAPI::rightClick(const QPoint &position) {
    desktop->warpCursor(position);
    desktop->pressMouse(Desktop::MouseButtons::right);
}

void DesktopAPI::pressMouse(const QPoint &position, const QString &button) {
    desktop->warpCursor(position);
    try {
        desktop->pressMouse(mouseButtonFromString(button));
    } catch (UnknownMouseButtonError &) {
        engine()->currentContext()->throwError(QScriptContext::RangeError,
                                               "Unknown button");
        return void();
    }
}

void DesktopAPI::holdMouse(const QPoint &position, const QString &button) {
    desktop->warpCursor(position);
    try {
        desktop->holdMouse(mouseButtonFromString(button));
    } catch (UnknownMouseButtonError &) {
        engine()->currentContext()->throwError(QScriptContext::RangeError,
                                               "Unknown button");
        return void();
    }
}

void DesktopAPI::releaseMouse(const QPoint &position, const QString &button) {
    desktop->warpCursor(position);
    try {
        desktop->releaseMouse(mouseButtonFromString(button));
    } catch (UnknownMouseButtonError &) {
        engine()->currentContext()->throwError(QScriptContext::RangeError,
                                               "Unknown button");
        return void();
    }
}

void DesktopAPI::pressKey(const QString &key) {
    // Don't call DesktopAPIs holdKey and releaseKey methods instead. Otherwise
    // a timeout might be missing between both methods. UinputFaker has such.
    // One is needed because of uinput events not beeing synchronious.
    if (!desktop->keyExists(key)) {
        engine()->currentContext()->throwError(QScriptContext::RangeError,
                                               "Unknown keycode.");
        return void();
    }
    desktop->pressKey(key);
}

void DesktopAPI::holdKey(const QString &key) {
    if (!desktop->keyExists(key)) {
        engine()->currentContext()->throwError(QScriptContext::RangeError,
                                               "Unknown keycode.");
        return void();
    }
    desktop->holdKey(key);
}

void DesktopAPI::releaseKey(const QString &key) {
    if (!desktop->keyExists(key)) {
        engine()->currentContext()->throwError(QScriptContext::RangeError,
                                               "Unknown keycode.");
        return void();
    }
    desktop->releaseKey(key);
}

void DesktopAPI::warpCursor(const QPoint &position) {
    desktop->warpCursor(position);
}

QPoint DesktopAPI::getCursorPosition() {
    int x = -1;
    int y = -1;
    if (!desktop->getCursorPosition(&x, &y)) {
        // Getting the cursor position failed.
    }
    return QPoint(x, y);
}

void DesktopAPI::scroll(int units, bool horizontal) {
    desktop->scroll(units, horizontal);
    return void();
}

Desktop::MouseButtons DesktopAPI::mouseButtonFromString(
    const QString &button) const {
    if (button.toLower() == "left") {
        return Desktop::MouseButtons::left;
    } else if (button.toLower() == "middle") {
        return Desktop::MouseButtons::middle;
    } else if (button.toLower() == "right") {
        return Desktop::MouseButtons::right;
    } else {
        throw UnknownMouseButtonError();
    }
}
