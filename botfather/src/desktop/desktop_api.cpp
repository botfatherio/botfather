#include "desktop_api.h"
#include <QPoint>
#include <QDebug>
#include "desktop.h"
#include "../vision/image.h"
#include "../vision/match.h"
#include "../vision/vision.h"
#include "../scripting/bot.h"
#include "../scripting/script_point.h"

DesktopAPI::DesktopAPI(Bot *bot_p, QScriptEngine *engine_p) : QObject(bot_p), m_bot_p(bot_p), m_engine_p(engine_p)
{
	desktop = new Desktop();
}

void DesktopAPI::enable(Bot *bot_p, QScriptEngine *engine_p)
{
	QScriptValue vision_obj = engine_p->newQObject(new DesktopAPI(bot_p, engine_p));
	engine_p->globalObject().setProperty("Desktop", vision_obj);
}

QScriptValue DesktopAPI::takeScreenshot()
{
	cv::UMat screenshot;
	if (!desktop->takeScreenshot(screenshot)) {
		// Taking the screenshot failed.
	}
	return m_engine_p->newQObject(new Image(screenshot));
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

void DesktopAPI::pressKey(QString key) {
	// Don't call DesktopAPIs holdKey and releaseKey methods instead. Otherwise a timeout might be
	// missing between both methods. UinputFaker has such. One is needed because of uinput events
	// not beeing synchronious.
	if (!desktop->keyExists(key)) {
		m_engine_p->currentContext()->throwError("Unknown keycode.");
		return;
	}
	desktop->pressKey(key);
}

void DesktopAPI::holdKey(QString key)
{
	if (!desktop->keyExists(key)) {
		m_engine_p->currentContext()->throwError("Unknown keycode.");
		return;
	}
	desktop->holdKey(key);
}

void DesktopAPI::releaseKey(QString key)
{
	if (!desktop->keyExists(key)) {
		m_engine_p->currentContext()->throwError("Unknown keycode.");
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
	return m_engine_p->newQObject(new ScriptPoint(x, y));
}

bool DesktopAPI::findAndClick(Image* tpl, double threshold, int button)
{
	cv::UMat screenshot;
	if (!desktop->takeScreenshot(screenshot) || !tpl) {
		return false;
	}
	if (screenshot.cols <= tpl->getWidth() || screenshot.rows <= tpl->getHeight()) {
		return false;
	}
	Match *match = Vision::findMatch(screenshot, tpl->getUMat(), threshold);
	switch (button) {
	case 1:
		leftClick(match->getX(), match->getY());
		break;
	case 2:
		middleClick(match->getX(), match->getY());
		break;
	case 3:
		rightClick(match->getX(), match->getY());
		break;
	default:
		m_engine_p->currentContext()->throwError("Unknown button code.");
		return false;
	}
	return true;
}