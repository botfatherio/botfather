#include "browser_api.h"
#include <QThread>
#include <QDebug>
#include "../modules/browser/browser.h"
#include "../modules/vision/vision.h"
#include "../prototypes/image_prototype.h"
#include "../types/match.h"
#include "vision_api.h"

QImage BrowserAPI::takeScreenshot()
{
	QImage qimage = Browser::takeScreenshot();
	engine()->reportAdditionalMemoryCost(ImageSizeInBytes(qimage));
	return qimage;
}

void BrowserAPI::blockResource(const QString &resource)
{
	Browser::blockResource(resource);
}

void BrowserAPI::replaceResource(const QString &old_resource, const QString &new_resource)
{
	Browser::replaceResource(old_resource, new_resource);
}

void BrowserAPI::unmodifyResource(const QString &resource)
{
	Browser::unmodifyResource(resource);
}

void BrowserAPI::unmodifyResources()
{
	Browser::unmodifyResources();
}

void BrowserAPI::loadUrl(const QString &url)
{
	Browser::loadUrl(url);
}

void BrowserAPI::beOnUrl(const QString &url)
{
	if (url != Browser::getUrl().toString()) {
		Browser::loadUrl(url);
	}
}

QUrl BrowserAPI::getUrl()
{
	return Browser::getUrl();
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

QSize BrowserAPI::getSize()
{
	return Browser::getSize();
}

void BrowserAPI::executeJavascript(const QString &javascript_code)
{
	Browser::executeJavascript(javascript_code);
}

void BrowserAPI::leftClick(const QPoint &position)
{
	Browser::pressMouse(MBT_LEFT, position);
	QThread::msleep(100);
	Browser::releaseMouse(MBT_LEFT, position);
}

void BrowserAPI::middleClick(const QPoint &position)
{
	Browser::pressMouse(MBT_MIDDLE, position);
	QThread::msleep(100);
	Browser::releaseMouse(MBT_MIDDLE, position);
}

void BrowserAPI::rightClick(const QPoint &position)
{
	Browser::pressMouse(MBT_RIGHT, position);
	QThread::msleep(100);
	Browser::releaseMouse(MBT_RIGHT, position);
}

void BrowserAPI::pressLeft(const QPoint &position)
{
	Browser::pressMouse(MBT_LEFT, position);
}

void BrowserAPI::pressMiddle(const QPoint &position)
{
	Browser::pressMouse(MBT_MIDDLE, position);
}

void BrowserAPI::pressRight(const QPoint &position)
{
	Browser::pressMouse(MBT_RIGHT, position);
}

void BrowserAPI::releaseLeft(const QPoint &position)
{
	Browser::releaseMouse(MBT_LEFT, position);
}

void BrowserAPI::releaseMiddle(const QPoint &position)
{
	Browser::releaseMouse(MBT_MIDDLE, position);
}

void BrowserAPI::releaseRight(const QPoint &position)
{
	Browser::releaseMouse(MBT_RIGHT, position);
}

void BrowserAPI::moveMouseTo(const QPoint &position)
{
	Browser::moveMouse(position);
}

void BrowserAPI::scrollWheel(const QPoint &position, int delta_x, int delta_y)
{
	Browser::scrollWheel(position, delta_x, delta_y);
}

bool BrowserAPI::findAndClick(const QImage &tpl, double threshold, int button)
{
	QImage screenshot = Browser::takeScreenshot();
	
	if (screenshot.isNull() || tpl.isNull()) {
		return false;
	}
	if (screenshot.width() <= tpl.width() || screenshot.height() <= tpl.height()) {
		return false;
	}
	
	cv::Mat ref_mat = Vision::qimageToBGRMat(screenshot);
	cv::Mat tpl_mat = Vision::qimageToBGRMat(tpl);
	
	Match match = Vision::findMatch(ref_mat, tpl_mat, threshold);
	
	ref_mat.release();
	tpl_mat.release();
	
	switch (button) {
	case 1:
		leftClick(match.center());
		break;
	case 2:
		middleClick(match.center());
		break;
	case 3:
		rightClick(match.center());
		break;
	default:
		engine()->currentContext()->throwError("Unknown button code.");
		return false;
	}
	return true;
}

QScriptValue BrowserAPI::findMatches(const QImage &tpl, double threshold, int max_matches)
{
    QImage screenshot = Browser::takeScreenshot();
    VisionAPI *vapi = new VisionAPI(bot(), engine());
	QScriptValue matches(vapi->findMatches(screenshot, tpl, threshold, max_matches));
	delete vapi;
	return matches;
}

QScriptValue BrowserAPI::findMatch(const QImage &tpl, double threshold)
{
    QImage screenshot = Browser::takeScreenshot();
    VisionAPI *vapi = new VisionAPI(bot(), engine());
	QScriptValue match(vapi->findMatch(screenshot, tpl, threshold));
	delete vapi;
    return match;
}
