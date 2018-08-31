#include "desktop_api.h"
#include <QPoint>
#include <QDebug>
#include "../modules/desktop/desktop.h"
#include "../modules/vision/vision.h"
#include "../types/match.h"
#include "vision_api.h"

DesktopAPI::DesktopAPI(Bot *bot, QObject *parent) : AbstractAPI(bot, parent)
{
	desktop = new Desktop(this);
}

QScriptValue DesktopAPI::takeScreenshot()
{
    QImage qimage = desktop->takeScreenshot();
    return engine()->toScriptValue(qimage);
}

int DesktopAPI::getWidth()
{
    return desktop->getWidth();
}

int DesktopAPI::getHeight()
{
    return desktop->getHeight();
}

void DesktopAPI::leftClick(int x, int y)
{
    desktop->leftClick(x, y);
}

void DesktopAPI::middleClick(int x, int y)
{
    desktop->middleClick(x, y);
}

void DesktopAPI::rightClick(int x, int y)
{
    desktop->rightClick(x, y);
}

void DesktopAPI::pressKey(const QString &key) {
    // Don't call DesktopAPIs holdKey and releaseKey methods instead. Otherwise a timeout might be
    // missing between both methods. UinputFaker has such. One is needed because of uinput events
    // not beeing synchronious.
    if (!desktop->keyExists(key)) {
	    engine()->currentContext()->throwError(QScriptContext::RangeError, "Unknown keycode.");
	    return;
    }
    desktop->pressKey(key);
}

void DesktopAPI::holdKey(const QString &key)
{
    if (!desktop->keyExists(key)) {
	    engine()->currentContext()->throwError(QScriptContext::RangeError, "Unknown keycode.");
	    return;
    }
    desktop->holdKey(key);
}

void DesktopAPI::releaseKey(const QString &key)
{
    if (!desktop->keyExists(key)) {
	    engine()->currentContext()->throwError(QScriptContext::RangeError, "Unknown keycode.");
	    return;
    }
    desktop->releaseKey(key);
}

void DesktopAPI::warpCursor(int x, int y)
{
    desktop->warpCursor(x, y);
}

QScriptValue DesktopAPI::getCursorPosition()
{
    int x = -1;
    int y = -1;
    if (!desktop->getCursorPosition(&x, &y)) {
	    // Getting the cursor position failed.
    }
    return engine()->toScriptValue(QPoint(x, y));
}

bool DesktopAPI::findAndClick(const QImage &tpl, double threshold, int button)
{
    QImage screenshot = desktop->takeScreenshot();
    if (screenshot.isNull() || tpl.isNull()) {
	    return false;
    }
    if (screenshot.width() <= tpl.width() || screenshot.height() <= tpl.height()) {
	    return false;
    }
    cv::Mat tpl_mat = Vision::qimageToBGRMat(tpl);
    cv::Mat screenshot_mat = Vision::qimageToBGRMat(screenshot);
    Match match = Vision::findMatch(screenshot_mat, tpl_mat, threshold);
    
    switch (button) {
    case 1:
	    leftClick(match.center().x(), match.center().y());
	    break;
    case 2:
	    middleClick(match.center().x(), match.center().y());
	    break;
    case 3:
	    rightClick(match.center().x(), match.center().y());
	    break;
    default:
	    engine()->currentContext()->throwError("Unknown button code.");
	    return false;
    }
    return true;
}

QScriptValue DesktopAPI::findMatches(const QImage &tpl, double threshold, int max_matches)
{
    QImage screenshot = desktop->takeScreenshot();
    VisionAPI *vapi = new VisionAPI(bot(), engine());
    QScriptValue matches = vapi->findMatches(screenshot, tpl, threshold, max_matches);
	delete vapi;
	return matches;
}

QScriptValue DesktopAPI::findMatch(const QImage &tpl, double threshold)
{
    QImage screenshot = desktop->takeScreenshot();
    VisionAPI *vapi = new VisionAPI(bot(), engine());
	QScriptValue match = vapi->findMatch(screenshot, tpl, threshold);
	delete vapi;
    return match;
}
