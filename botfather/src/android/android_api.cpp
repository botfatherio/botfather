#include "android_api.h"
#include "adb_wrapper.h"
#include "android_settings.h"
#include "../vision/vision.h"
#include "../vision/image.h"
#include "../vision/match.h"
#include "../scripting/bot.h"

AndroidAPI::AndroidAPI(Bot* bot_p, QScriptEngine* engine_p) : QObject(bot_p), m_engine_p(engine_p)
{
	QString adb_binary = m_settings.value(android::options::ADB_BINARY).toString();
	adb = new AdbWrapper(this, adb_binary);
	adb->startAdbServer();
	serial_number = m_settings.value(android::options::SERIAL_NUMBER).toString();
}

void AndroidAPI::enable(Bot* bot_p, QScriptEngine *engine_p)
{
	QScriptValue vision_obj = engine_p->newQObject(new AndroidAPI(bot_p, engine_p));
	engine_p->globalObject().setProperty("Android", vision_obj);
}

bool AndroidAPI::connected()
{
	QList<AdbDeviceInfo> devices;
	adb->queryForDevices(devices);
	for (AdbDeviceInfo device : devices) {
		if (device.getSerialNumber() == serial_number) {
			return device.connected();
		}
	}
	return false;
}

QScriptValue AndroidAPI::listPackages()
{
	QList<QString> packages;
	if (!adb->listPackages(serial_number, packages)) {
		// Listing the packages failed. A empty list will be returned.
	}
	return qScriptValueFromSequence(m_engine_p, packages);
}

bool AndroidAPI::startApp(QString package)
{
	return adb->startApp(serial_number, package);
}

bool AndroidAPI::sendTap(int x, int y)
{
	return adb->sendTap(serial_number, x, y);
}

bool AndroidAPI::sendSwipe(int x1, int y1, int x2, int y2, int duration_in_ms)
{
	if (duration_in_ms < 0) {
		m_engine_p->currentContext()->throwError("Duration must be positive.");
		return false;
	} 
	return adb->sendSwipe(serial_number, x1, y1, x2, y2, duration_in_ms);
}

bool AndroidAPI::sendEvent(QString device, int type, int code, int value)
{
	return adb->sendEvent(serial_number, device, type, code, value);
}

bool AndroidAPI::sendKeyEvent(QString key_event_code)
{
	return adb->sendKeyEvent(serial_number, key_event_code);
}

bool AndroidAPI::sendTextInput(QString text)
{
	return adb->sendTextInput(serial_number, text);
}

QScriptValue AndroidAPI::takeScreenshot()
{
	QImage qimage;
	if (!adb->takeScreenshot(serial_number, qimage)) {
		// Taking screenshot failed. TODO: print system debug info in some later version...
	}
	cv::UMat umat = Vision::qimageToBGRUmat(qimage).clone();
	return m_engine_p->newQObject(new Image(umat));
}

int AndroidAPI::getDeviceWidth()
{
	QImage qimage;
	adb->takeScreenshot(serial_number, qimage);
	if (qimage.isNull()) {
		return -1;
	}
	return qimage.width();
}

int AndroidAPI::getDeviceHeight()
{
	QImage qimage;
	adb->takeScreenshot(serial_number, qimage);
	if (qimage.isNull()) {
		return -1;
	}
	return qimage.height();
}

bool AndroidAPI::findAndTap(Image* tpl, double threshold)
{
	QImage qimage;
	if (!adb->takeScreenshot(serial_number, qimage) || !tpl) {
		return false;
	}
	cv::UMat screenshot = Vision::qimageToBGRUmat(qimage).clone();
	if (screenshot.cols <= tpl->getWidth() || screenshot.rows <= tpl->getHeight()) {
		return false;
	}
	Match *match = Vision::findMatch(screenshot, tpl->getUMat(), threshold);
	sendTap(match->getX(), match->getY());
	return true;
}