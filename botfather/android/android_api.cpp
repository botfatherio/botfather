#include "android_api.h"
#include <QSettings>
#include "adb_wrapper.h"
#include "../vision/vision.h"
#include "../vision/image.h"
#include "../scripting/bot.h"

AndroidAPI::AndroidAPI(Bot* bot_p, QJSEngine* engine_p) : QObject(bot_p), m_engine_p(engine_p)
{
	QString adb_binary = m_settings.value("Android/adb_binary", "").toString();
	adb = new AdbWrapper(this, adb_binary);
	adb->startAdbServer();
	serial_number = m_settings.value("Android/serial_number", "").toString();
}

void AndroidAPI::enable(Bot* bot_p, QJSEngine *engine_p)
{
	QJSValue vision_obj = engine_p->newQObject(new AndroidAPI(bot_p, engine_p));
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

QJSValue AndroidAPI::listPackages()
{
	QList<QString> packages;
	if (!adb->listPackages(serial_number, packages)) {
		// Listing the packages failed. TODO: Return a QJSValue Exception thingy?
	}
	
	// Transform the packages list into and JS array of packages.
	QJSValue js_packages = m_engine_p->newArray();
	for (int i = 0; i < packages.size(); i++){
		js_packages.setProperty(i, packages[i]);
	}
	return js_packages;
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
	return adb->sendSwipe(serial_number, x1, y1, x2, y2, duration_in_ms);
}

bool AndroidAPI::sendKeyevent(QString key_event_code)
{
	return adb->sendKeyevent(serial_number, key_event_code);
}

bool AndroidAPI::sendTextInput(QString text)
{
	return adb->sendTextInput(serial_number, text);
}

QJSValue AndroidAPI::takeScreenshot()
{
	QImage qimage;
	if (!adb->takeScreenshot(serial_number, qimage)) {
		// Taking screenshot failed. TODO: print system debug info in some later version...
	}
	cv::UMat umat = Vision::qimageToUmat(qimage).clone();
	return m_engine_p->newQObject(new Image(umat));
}