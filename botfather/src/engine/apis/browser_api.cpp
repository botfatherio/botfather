#include "browser_api.h"
#include <QThread>
#include <QDebug>
#include "../modules/browser/browser.h"
#include "../modules/vision/vision.h"
#include "../types/match.h"
#include "vision_api.h"

QScriptValue BrowserAPI::takeScreenshot()
{
	QImage qimage = Browser::takeScreenshot();
	// NOTE: It's okay to report additional cost before creating the new object.
	// Because we can expect the new object to not be instantly garbage...
	//m_engine_p->reportAdditionalMemoryCost(static_cast<int>(ImageSizeInBytes(qimage)));
	return engine()->toScriptValue(qimage);
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

bool BrowserAPI::findAndClick(QImage* tpl, double threshold, int button)
{
	QImage screenshot = Browser::takeScreenshot();
	
	if (screenshot.isNull() || !tpl) {
		return false;
	}
	if (screenshot.width() <= tpl->width() || screenshot.height() <= tpl->height()) {
		return false;
	}
	
	cv::Mat ref_mat = Vision::qimageToBGRMat(screenshot);
	cv::Mat tpl_mat = Vision::qimageToBGRMat(*tpl); // Check whether this works or leaks mem
	
	Match match = Vision::findMatch(ref_mat, tpl_mat, threshold);
	
	ref_mat.release();
	tpl_mat.release();
	
	
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

QScriptValue BrowserAPI::findMatches(QImage* tpl, double threshold, int max_matches)
{
	QImage screenshot = Browser::takeScreenshot();
	VisionAPI *vapi = new VisionAPI(bot(), engine());
	return vapi->findMatches(&screenshot, tpl, threshold, max_matches); // FIXME: this pointer will leak mem.
}

QScriptValue BrowserAPI::findMatch(QImage* tpl, double threshold)
{
	QImage screenshot = Browser::takeScreenshot();
	VisionAPI *vapi = new VisionAPI(bot(), engine());
	return vapi->findMatch(&screenshot, tpl, threshold); // FIXME: this pointer will leak mem.
}