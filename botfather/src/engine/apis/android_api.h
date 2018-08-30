#ifndef BFP_ENGINE_APIS_ANDROID_API_H
#define BFP_ENGINE_APIS_ANDROID_API_H

#include <QObject>
#include <QScriptEngine>
#include <QImage>
#include <QString>
#include <QSettings>
#include "abstract_api.h"

class AdbWrapper;

class AndroidAPI : public AbstractAPI
{
	Q_OBJECT
public:
	AndroidAPI(Bot *bot, QObject *parent = nullptr);
	
	Q_INVOKABLE bool connected();
	Q_INVOKABLE QScriptValue listPackages();
	Q_INVOKABLE bool startApp(const QString &package);
	Q_INVOKABLE bool sendTap(int x, int y);
	Q_INVOKABLE bool sendSwipe(int x1, int y1, int x2, int y2, int duration_in_ms = 500);
	Q_INVOKABLE bool sendEvent(const QString &device, int type, int code, int value);
	Q_INVOKABLE bool sendKeyEvent(const QString &key_event_code); // https://developer.android.com/reference/android/view/KeyEvent.html
	Q_INVOKABLE bool sendTextInput(const QString &text);
	Q_INVOKABLE QScriptValue takeScreenshot();
	
	// Returns the android devices screenshot width or -1 if it couldn't be obtained.
	Q_INVOKABLE int getDeviceWidth();
	
	// Returns the android devices screenshot height or -1 if it couldn't be obtained.
	Q_INVOKABLE int getDeviceHeight();
	
	Q_INVOKABLE bool findAndTap(const QImage &tpl, double threshold = 0.8);
	Q_INVOKABLE QScriptValue findMatches(const QImage &tpl, double threshold = 0.8, int max_matches = -1);
	Q_INVOKABLE QScriptValue findMatch(const QImage &tpl, double threshold = 0.8);
	
private:
	QSettings m_settings;
	AdbWrapper *adb;
	QString serial_number;
};

#endif // BFP_ENGINE_APIS_ANDROID_API_H
