#include "android_api.hpp"

#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <QTimer>

#include "../../../engine.hpp"
#include "../../android/android_settings.hpp"
#include "../../vision/apis/vision_api.hpp"
#include "../../vision/prototypes/image_prototype.hpp"
#include "../../vision/types/match.hpp"
#include "../../vision/vision.hpp"

AndroidAPI::AndroidAPI(Engine *bot, QObject *parent)
    : AbstractAPI(bot, parent) {
    QStringList search_paths;
    search_paths << QApplication::applicationDirPath()
                 << QDir(QApplication::applicationDirPath()).filePath("adb");

    QString adb_binary =
        QStandardPaths::findExecutable("adb", search_paths);  // default
    QSettings settings;

    if (settings
            .value(android::options::USE_CUSTOM_ADB,
                   android::fallback::USE_CUSTOM_ADB)
            .toBool()) {
        adb_binary = settings.value(android::options::ADB_BINARY).toString();
    }

    adb = new AdbWrapper(this, adb_binary);
    serial_number =
        m_settings.value(android::options::SERIAL_NUMBER).toString();
    QTimer::singleShot(1, adb, &AdbWrapper::startAdbServer);
}

bool AndroidAPI::connected() {
    QVector<AdbDeviceInfo> devices;
    adb->queryForDevices(devices);
    for (AdbDeviceInfo device : devices) {
        if (device.getSerialNumber() == serial_number) {
            return device.connected();
        }
    }
    return false;
}

QScriptValue AndroidAPI::listPackages() {
    QVector<QString> packages;
    if (!adb->listPackages(serial_number, packages)) {
        // Listing the packages failed. A empty list will be returned.
    }
    return qScriptValueFromSequence(engine(), packages);
}

bool AndroidAPI::startApp(const QString &package) {
    return adb->startApp(serial_number, package);
}

bool AndroidAPI::sendTap(const QPoint &location) {
    return adb->sendTap(serial_number, location.x(), location.y());
}

bool AndroidAPI::sendSwipe(const QPoint &start, const QPoint &end,
                           int duration_in_ms) {
    if (duration_in_ms < 0) {
        engine()->currentContext()->throwError(QScriptContext::RangeError,
                                               "Duration must be positive.");
        return false;
    }
    return adb->sendSwipe(serial_number, start.x(), start.y(), end.x(), end.y(),
                          duration_in_ms);
}

bool AndroidAPI::sendEvent(const QString &input_device, int type, int code,
                           int value) {
    return adb->sendEvent(serial_number, input_device, type, code, value);
}

bool AndroidAPI::sendKeyEvent(const QString &key_event_code) {
    return adb->sendKeyEvent(serial_number, key_event_code);
}

bool AndroidAPI::sendTextInput(const QString &text) {
    return adb->sendTextInput(serial_number, text);
}

QImage AndroidAPI::takeScreenshot() {
    QImage qimage;
    adb->takeScreenshot(serial_number, qimage);
    engine()->reportAdditionalMemoryCost(ImageSizeInBytes(qimage));
    return qimage;
}

QSize AndroidAPI::getSize() {
    QImage qimage;
    adb->takeScreenshot(serial_number, qimage);
    return qimage.size();
}

QRect AndroidAPI::getRect() { return QRect(QPoint(0, 0), getSize()); }
