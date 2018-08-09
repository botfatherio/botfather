#include "desktop_api.h"
#include <QPoint>
#include <QDebug>
#include "desktop.h"
#include "../vision/image.h"
#include "../vision/match.h"
#include "../vision/vision.h"
#include "../vision/vision_api.h"
#include "../scripting/bot.h"
#include "../scripting/script_point.h"

DesktopAPI::DesktopAPI(Bot *bot_p, QScriptEngine *engine_p) : QObject(bot_p), m_bot_p(bot_p), m_engine_p(engine_p)
{
	desktop = new Desktop(this);
}

void DesktopAPI::enable(Bot *bot_p, QScriptEngine *engine_p)
{
	QScriptValue vision_obj = engine_p->newQObject(new DesktopAPI(bot_p, engine_p), QScriptEngine::ScriptOwnership);
	engine_p->globalObject().setProperty("Desktop", vision_obj);
}

QScriptValue DesktopAPI::takeScreenshot()
{
	QImage qimage = desktop->takeScreenshot();
	m_engine_p->reportAdditionalMemoryCost(static_cast<int>(qimage.sizeInBytes()));
	return m_engine_p->newQObject(new Image(qimage), QScriptEngine::ScriptOwnership);
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
	return m_engine_p->newQObject(new ScriptPoint(x, y), QScriptEngine::ScriptOwnership);
}

bool DesktopAPI::findAndClick(Image* tpl, double threshold, int button)
{
	QImage screenshot = desktop->takeScreenshot();
	if (screenshot.isNull() || !tpl) {
		return false;
	}
	if (screenshot.width() <= tpl->getWidth() || screenshot.height() <= tpl->getHeight()) {
		return false;
	}
	cv::Mat tpl_mat = Vision::qimageToBGRMat(tpl->getQImage());
	cv::Mat screenshot_mat = Vision::qimageToBGRMat(screenshot);
	Match *match = Vision::findMatch(screenshot_mat, tpl_mat, threshold);
	
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

QScriptValue DesktopAPI::findMatches(Image* tpl, double threshold, int max_matches)
{
	VisionAPI *vapi = new VisionAPI(m_bot_p, m_engine_p);
	Image *screenshot = qscriptvalue_cast<Image*>(takeScreenshot());
	return vapi->findMatches(screenshot, tpl, threshold, max_matches);
}

QScriptValue DesktopAPI::findMatch(Image* tpl, double threshold)
{
	VisionAPI *vapi = new VisionAPI(m_bot_p, m_engine_p);
	Image *screenshot = qscriptvalue_cast<Image*>(takeScreenshot());
	return vapi->findMatch(screenshot, tpl, threshold);
}