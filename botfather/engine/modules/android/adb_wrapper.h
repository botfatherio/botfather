#ifndef BFP_ENGINE_MODULES_ANDROID_ADB_WRAPPER_H
#define BFP_ENGINE_MODULES_ANDROID_ADB_WRAPPER_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QProcess>
#include <QImage>
#include "adb_device_info.h"

class AdbWrapper : public QObject
{
	Q_OBJECT
public:
	explicit AdbWrapper(QObject *parent = nullptr, QString adb_binary = QString());
	
public slots:
	bool startAdbServer();

public:
	bool queryForDevices(QVector<AdbDeviceInfo> &devices);
	bool listPackages(QString serial_number, QVector<QString> &packages);
	bool startApp(QString serial_number, QString package);
	bool sendTap(QString serial_number, int x, int y);
	bool sendSwipe(QString serial_number, int x1, int y1, int x2, int y2, int duration_in_ms);
	bool sendKeyEvent(QString serial_number, QString key_event_code); // https://developer.android.com/reference/android/view/KeyEvent.html
	bool sendEvent(QString serial_number, QString device, int type, int code, int value);
	bool sendTextInput(QString serial_number, QString text);
	bool takeScreenshot(QString serial_number, QImage &screenshot);
	
private:
	QString adb_binary;
	
	bool evaluateAdbCommand(QStringList arguments, int timeout_in_ms = 30000);
	bool evaluateAdbCommand(QStringList arguments, QString &standard_output, int timeout_in_ms = 30000);
	QProcess::ProcessError executeAdbCommand(QStringList arguments, QString &standard_output, QString &standard_error, int timeout_in_ms = 30000);
	QProcess::ProcessError executeAdbCommand(QStringList arguments, QByteArray &standard_output, QByteArray &standard_error, int timeout_in_ms = 30000);
};

#endif // BFP_ENGINE_MODULES_ANDROID_ADB_WRAPPER_H
