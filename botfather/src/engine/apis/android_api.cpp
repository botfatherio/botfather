#include "android_api.h"
#include "../modules/android/adb_wrapper.h"
#include "../modules/android/android_settings.h"
#include "../modules/vision/vision.h"
#include "../prototypes/image_prototype.h"
#include "../types/match.h"
#include "../bot.h"
#include "vision_api.h"

AndroidAPI::AndroidAPI(Bot *bot, QObject *parent) : AbstractAPI(bot, parent)
{
	QString adb_binary = m_settings.value(android::options::ADB_BINARY).toString();
	adb = new AdbWrapper(this, adb_binary);
	adb->startAdbServer();
	serial_number = m_settings.value(android::options::SERIAL_NUMBER).toString();
}

bool AndroidAPI::connected()
{
	QVector<AdbDeviceInfo> devices;
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
	QVector<QString> packages;
	if (!adb->listPackages(serial_number, packages)) {
		// Listing the packages failed. A empty list will be returned.
	}
	return qScriptValueFromSequence(engine(), packages);
}

bool AndroidAPI::startApp(const QString &package)
{
	return adb->startApp(serial_number, package);
}

bool AndroidAPI::sendTap(const QPoint &location)
{
	return adb->sendTap(serial_number, location.x(), location.y());
}

bool AndroidAPI::sendSwipe(const QPoint &start, const QPoint &end, int duration_in_ms)
{
	if (duration_in_ms < 0) {
		engine()->currentContext()->throwError(QScriptContext::RangeError, "Duration must be positive.");
		return false;
	} 
	return adb->sendSwipe(serial_number, start.x(), start.y(), end.x(), end.y(), duration_in_ms);
}

bool AndroidAPI::sendEvent(const QString &input_device, int type, int code, int value)
{
	return adb->sendEvent(serial_number, input_device, type, code, value);
}

bool AndroidAPI::sendKeyEvent(const QString &key_event_code)
{
	return adb->sendKeyEvent(serial_number, key_event_code);
}

bool AndroidAPI::sendTextInput(const QString &text)
{
	return adb->sendTextInput(serial_number, text);
}

QImage AndroidAPI::takeScreenshot()
{
	QImage qimage;
	adb->takeScreenshot(serial_number, qimage);
	engine()->reportAdditionalMemoryCost(ImageSizeInBytes(qimage));
	return qimage;
}

QSize AndroidAPI::getSize()
{
	QImage qimage;
	adb->takeScreenshot(serial_number, qimage);
	return qimage.size();
}

QRect AndroidAPI::getRect()
{
	return QRect(QPoint(0, 0), getSize());
}

bool AndroidAPI::findAndTap(const QImage &tpl, double threshold)
{
	QImage qimage;
	if (!adb->takeScreenshot(serial_number, qimage) || tpl.isNull()) {
		return false;
	}
	if (qimage.width() <= tpl.width() || qimage.height() <= tpl.height()) {
		return false;
	}

	cv::Mat image_mat = Vision::qimageToBGRMat(qimage);
	cv::Mat tpl_mat = Vision::qimageToBGRMat(tpl);

	Match match = Vision::findMatch(image_mat, tpl_mat, threshold);
	sendTap(match.center());

	image_mat.release();
	tpl_mat.release();

	return true;
}

QScriptValue AndroidAPI::findMatches(const QImage &tpl, double threshold, int max_matches)
{
	QImage screenshot;
	adb->takeScreenshot(serial_number, screenshot);
    VisionAPI *vapi = new VisionAPI(bot(), engine());
	QScriptValue matches(vapi->findMatches(screenshot, tpl, threshold, max_matches));
	delete vapi;
	return matches;
}

QScriptValue AndroidAPI::findMatch(const QImage &tpl, double threshold)
{
	QImage screenshot;
	adb->takeScreenshot(serial_number, screenshot);
    VisionAPI *vapi = new VisionAPI(bot(), engine());
	QScriptValue match(vapi->findMatch(screenshot, tpl, threshold));
	delete vapi;
    return match;
}
