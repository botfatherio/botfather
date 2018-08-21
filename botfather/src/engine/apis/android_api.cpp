#include "android_api.h"
#include "../modules/android/adb_wrapper.h"
#include "../modules/android/android_settings.h"
#include "../modules/vision/vision.h"
#include "../types/match.h"
#include "../bot.h"
#include "vision_api.h"

AndroidAPI::AndroidAPI(Bot* bot_p, QScriptEngine* engine_p)
	: QObject(bot_p)
	, m_bot_p(bot_p)
	, m_engine_p(engine_p)
{
	QString adb_binary = m_settings.value(android::options::ADB_BINARY).toString();
	adb = new AdbWrapper(this, adb_binary);
	adb->startAdbServer();
	serial_number = m_settings.value(android::options::SERIAL_NUMBER).toString();
}

void AndroidAPI::enable(Bot* bot_p, QScriptEngine *engine_p)
{
	QScriptValue vision_obj = engine_p->newQObject(new AndroidAPI(bot_p, engine_p), QScriptEngine::ScriptOwnership);
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
	//m_engine_p->reportAdditionalMemoryCost(static_cast<int>(ImageSizeInBytes(qimage)));
	return m_engine_p->toScriptValue(qimage);
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

bool AndroidAPI::findAndTap(QImage* tpl, double threshold)
{
	QImage qimage;
	if (!adb->takeScreenshot(serial_number, qimage) || !tpl) {
		return false;
	}
	if (qimage.width() <= tpl->width() || qimage.height() <= tpl->height()) {
		return false;
	}
	cv::Mat image_mat = Vision::qimageToBGRMat(qimage);
	cv::Mat tpl_mat = Vision::qimageToBGRMat(*tpl); // TODO: Check whether this works or leaks mem
	Match match = Vision::findMatch(image_mat, tpl_mat, threshold);
	sendTap(match.center().x(), match.center().y());
	return true;
}

QScriptValue AndroidAPI::findMatches(QImage* tpl, double threshold, int max_matches)
{
	QImage screenshot;
	adb->takeScreenshot(serial_number, screenshot);
	VisionAPI *vapi = new VisionAPI(m_bot_p, m_engine_p);
	return vapi->findMatches(&screenshot, tpl, threshold, max_matches); // FIXME: this pointer will leak mem
}

QScriptValue AndroidAPI::findMatch(QImage* tpl, double threshold)
{
	QImage screenshot;
	adb->takeScreenshot(serial_number, screenshot);
	VisionAPI *vapi = new VisionAPI(m_bot_p, m_engine_p);
	return vapi->findMatch(&screenshot, tpl, threshold); // FIXME: this pointer will leak mem
}