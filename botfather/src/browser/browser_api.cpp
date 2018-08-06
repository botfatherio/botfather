#include "browser_api.h"
#include <QThread>
#include <QDebug>
#include "browser.h"
#include "../vision/vision.h"
#include "../vision/vision_api.h"
#include "../vision/image.h"
#include "../vision/match.h"
#include "../scripting/bot.h"

BrowserAPI::BrowserAPI(Bot *bot_p, QScriptEngine *engine_p) : QObject(bot_p),  m_bot_p(bot_p), m_engine_p(engine_p)
{}

void BrowserAPI::enable(Bot *bot_p, QScriptEngine *engine_p)
{
	QScriptValue vision_obj = engine_p->newQObject(new BrowserAPI(bot_p, engine_p), QScriptEngine::ScriptOwnership);
	engine_p->globalObject().setProperty("Browser", vision_obj);
}

QScriptValue BrowserAPI::takeScreenshot()
{
	QImage qimage = Browser::takeScreenshot();
	// NOTE: It's okay to report additional cost before creating the new object.
	// Because we can expect the new object to not be instantly garbage...
	m_engine_p->reportAdditionalMemoryCost(static_cast<int>(qimage.sizeInBytes()));
	return m_engine_p->newQObject(new Image(qimage), QScriptEngine::ScriptOwnership);
}

void BrowserAPI::blockResource(QString resource)
{
	Browser::blockResource(resource);
}

void BrowserAPI::replaceResource(QString old_resource, QString new_resource)
{
	Browser::replaceResource(old_resource, new_resource);
}

void BrowserAPI::unmodifyResource(QString resource)
{
	Browser::unmodifyResource(resource);
}

void BrowserAPI::unmodifyResources()
{
	Browser::unmodifyResources();
}

void BrowserAPI::loadUrl(QString url)
{
	Browser::loadUrl(url);
}

void BrowserAPI::beOnUrl(QString url)
{
	if (url != Browser::getUrl().toString()) {
		Browser::loadUrl(url);
	}
}

QString BrowserAPI::getUrl()
{
	return Browser::getUrl().toString();
}

void BrowserAPI::reload()
{
	Browser::reload();
}

void BrowserAPI::reloadIgnoringCache()
{
	Browser::reloadIgnoringCache();
}

bool BrowserAPI::loading()
{
	return Browser::loading();
}

bool BrowserAPI::finishLoading(int timeout_seconds)
{
	return Browser::finishLoading(timeout_seconds);
}

void BrowserAPI::stopLoading()
{
	Browser::stopLoad();
}

bool BrowserAPI::canGoBack()
{
	return Browser::canGoBack();
}

bool BrowserAPI::canGoForward()
{
	return Browser::canGoForward();
}

void BrowserAPI::goBack()
{
	Browser::goBack();
}

void BrowserAPI::goForward()
{
	Browser::goForward();
}

int BrowserAPI::getWidth()
{
	return Browser::getWidth();
}

int BrowserAPI::getHeight()
{
	return Browser::getHeight();
}

void BrowserAPI::executeJavascript(QString javascript_code)
{
	Browser::executeJavascript(javascript_code);
}

void BrowserAPI::leftClick(int x, int y)
{
	Browser::pressMouse(MBT_LEFT, x, y);
	QThread::msleep(100);
	Browser::releaseMouse(MBT_LEFT, x, y);
}

void BrowserAPI::middleClick(int x, int y)
{
	Browser::pressMouse(MBT_MIDDLE, x, y);
	QThread::msleep(100);
	Browser::releaseMouse(MBT_MIDDLE, x, y);
}

void BrowserAPI::rightClick(int x, int y)
{
	Browser::pressMouse(MBT_RIGHT, x, y);
	QThread::msleep(100);
	Browser::releaseMouse(MBT_RIGHT, x, y);
}

void BrowserAPI::pressLeft(int x, int y)
{
	Browser::pressMouse(MBT_LEFT, x, y);
}

void BrowserAPI::pressMiddle(int x, int y)
{
	Browser::pressMouse(MBT_MIDDLE, x, y);
}

void BrowserAPI::pressRight(int x, int y)
{
	Browser::pressMouse(MBT_RIGHT, x, y);
}

void BrowserAPI::releaseLeft(int x, int y)
{
	Browser::releaseMouse(MBT_LEFT, x, y);
}

void BrowserAPI::releaseMiddle(int x, int y)
{
	Browser::releaseMouse(MBT_MIDDLE, x, y);
}

void BrowserAPI::releaseRight(int x, int y)
{
	Browser::releaseMouse(MBT_RIGHT, x, y);
}

void BrowserAPI::moveMouseTo(int x, int y)
{
	Browser::moveMouse(x, y);
}

void BrowserAPI::scrollWheel(int x, int y, int delta_x, int delta_y)
{
	Browser::scrollWheel(x, y, delta_x, delta_y);
}

bool BrowserAPI::findAndClick(Image* tpl, double threshold, int button)
{
	QImage screenshot = Browser::takeScreenshot();
	
	if (screenshot.isNull() || !tpl) {
		return false;
	}
	if (screenshot.width() <= tpl->getWidth() || screenshot.height() <= tpl->getHeight()) {
		return false;
	}
	
	cv::Mat ref_mat = Vision::qimageToBGRMat(screenshot);
	cv::Mat tpl_mat = Vision::qimageToBGRMat(tpl->getQImage());
	
	Match *match = Vision::findMatch(ref_mat, tpl_mat, threshold);
	
	ref_mat.release();
	tpl_mat.release();
	
	
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

QScriptValue BrowserAPI::findMatches(Image* tpl, double threshold, int max_matches)
{
	VisionAPI *vapi = new VisionAPI(m_bot_p, m_engine_p);
	Image *screenshot = qscriptvalue_cast<Image*>(takeScreenshot());
	return vapi->findMatches(screenshot, tpl, threshold, max_matches);
}

QScriptValue BrowserAPI::findMatch(Image* tpl, double threshold)
{
	VisionAPI *vapi = new VisionAPI(m_bot_p, m_engine_p);
	Image *screenshot = qscriptvalue_cast<Image*>(takeScreenshot());
	return vapi->findMatch(screenshot, tpl, threshold);
}