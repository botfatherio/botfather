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

	Q_INVOKABLE QImage takeScreenshot();
	
	Q_INVOKABLE bool connected();

	Q_INVOKABLE QScriptValue listPackages();
	Q_INVOKABLE bool startApp(const QString &package);

	Q_INVOKABLE bool sendTap(const QPoint &location);
	Q_INVOKABLE bool sendSwipe(const QPoint &start, const QPoint &end, int duration_in_ms = 500);
	Q_INVOKABLE bool sendEvent(const QString &input_device, int type, int code, int value);
	Q_INVOKABLE bool sendKeyEvent(const QString &key_event_code); // https://developer.android.com/reference/android/view/KeyEvent.html
	Q_INVOKABLE bool sendTextInput(const QString &text);
	
	// Returns the android devices screen size. It's usefull to know the screens size to choose what templates to use.
	Q_INVOKABLE QSize getSize();
	Q_INVOKABLE QRect getRect();
	
	Q_INVOKABLE bool findAndTap(const QImage &tpl, double threshold = 0.8);
	Q_INVOKABLE QScriptValue findMatches(const QImage &tpl, double threshold = 0.8, int max_matches = -1);
	Q_INVOKABLE QScriptValue findMatch(const QImage &tpl, double threshold = 0.8);
	
private:
	QSettings m_settings;
	AdbWrapper *adb;
	QString serial_number;
};

#endif // BFP_ENGINE_APIS_ANDROID_API_H
