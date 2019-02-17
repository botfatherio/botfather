#include "browser_api.h"
#include <QThread>
#include <QRegularExpression>
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

void BrowserAPI::blockResource(const QString &resource_pattern)
{
	QRegularExpression regex(resource_pattern);
	if (!regex.isValid())
	{
		engine()->currentContext()->throwError(QScriptContext::TypeError, "Invalid regular expression");
		return;
	}
	Browser::blockResource(resource_pattern);
}

void BrowserAPI::replaceResource(const QString &resource_pattern, const QString &new_resource)
{
	QRegularExpression regex(resource_pattern);
	if (!regex.isValid())
	{
		engine()->currentContext()->throwError(QScriptContext::TypeError, "Invalid regular expression");
		return;
	}
	Browser::replaceResource(resource_pattern, new_resource);
}

void BrowserAPI::unmodifyResource(const QString &resource_pattern)
{
	Browser::unmodifyResource(resource_pattern);
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

QRect BrowserAPI::getRect()
{
	return QRect(QPoint(0, 0), Browser::getSize());
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
