#include "desktop_api.h"
#include <QPoint>
#include <QDebug>
#include "../modules/desktop/desktop.h"
#include "../modules/vision/vision.h"
#include "../prototypes/image_prototype.h"
#include "../types/match.h"
#include "vision_api.h"

DesktopAPI::DesktopAPI(Bot *bot, QObject *parent) : AbstractAPI(bot, parent)
{
	desktop = new Desktop(this);
}

QImage DesktopAPI::takeScreenshot()
{
    QImage qimage = desktop->takeScreenshot();
	engine()->reportAdditionalMemoryCost(ImageSizeInBytes(qimage));
	return qimage;
}

QSize DesktopAPI::getSize()
{
	return desktop->getSize();
}

QRect DesktopAPI::getRect()
{
	return QRect(QPoint(0, 0), desktop->getSize());
}

void DesktopAPI::leftClick(const QPoint &position)
{
	desktop->leftClick(position);
}

void DesktopAPI::middleClick(const QPoint &position)
{
	desktop->middleClick(position);
}

void DesktopAPI::rightClick(const QPoint &position)
{
	desktop->rightClick(position);
}

void DesktopAPI::pressKey(const QString &key) {
    // Don't call DesktopAPIs holdKey and releaseKey methods instead. Otherwise a timeout might be
    // missing between both methods. UinputFaker has such. One is needed because of uinput events
    // not beeing synchronious.
    if (!desktop->keyExists(key)) {
	    engine()->currentContext()->throwError(QScriptContext::RangeError, "Unknown keycode.");
		return void();
    }
    desktop->pressKey(key);
}

void DesktopAPI::holdKey(const QString &key)
{
    if (!desktop->keyExists(key)) {
	    engine()->currentContext()->throwError(QScriptContext::RangeError, "Unknown keycode.");
		return void();
    }
    desktop->holdKey(key);
}

void DesktopAPI::releaseKey(const QString &key)
{
    if (!desktop->keyExists(key)) {
	    engine()->currentContext()->throwError(QScriptContext::RangeError, "Unknown keycode.");
		return void();
    }
    desktop->releaseKey(key);
}

void DesktopAPI::warpCursor(const QPoint &position)
{
	desktop->warpCursor(position);
}

QPoint DesktopAPI::getCursorPosition()
{
    int x = -1;
    int y = -1;
    if (!desktop->getCursorPosition(&x, &y)) {
	    // Getting the cursor position failed.
    }
	return QPoint(x, y);
}
