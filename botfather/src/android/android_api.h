#ifndef BFP__ANDROID__ANDROID_API_H
#define BFP__ANDROID__ANDROID_API_H

#include <QObject>
#include <QJSEngine>
#include <QString>
#include <QSettings>

class Bot;
class AdbWrapper;

class AndroidAPI : public QObject
{
	Q_OBJECT
public:
	explicit AndroidAPI(Bot* bot_p, QJSEngine* engine_p);
	static void enable(Bot* bot_p, QJSEngine* engine_p);
	
	Q_INVOKABLE bool connected();
	Q_INVOKABLE QJSValue listPackages();
	Q_INVOKABLE bool startApp(QString package);
	Q_INVOKABLE bool sendTap(int x, int y);
	Q_INVOKABLE bool sendSwipe(int x1, int y1, int x2, int y2, int duration_in_ms);
	Q_INVOKABLE bool sendKeyevent(QString key_event_code); // https://developer.android.com/reference/android/view/KeyEvent.html
	Q_INVOKABLE bool sendTextInput(QString text);
	Q_INVOKABLE QJSValue takeScreenshot();
	
	// Returns the android devices screenshot width or -1 if it couldn't be obtained.
	Q_INVOKABLE int getDeviceWidth();
	
	// Returns the android devices screenshot height or -1 if it couldn't be obtained.
	Q_INVOKABLE int getDeviceHeight();
	
private:
	QJSEngine *m_engine_p;
	QSettings m_settings;
	AdbWrapper *adb;
	QString serial_number;
};

#endif // BFP__ANDROID__ANDROID_API_H