#ifndef BFP_ENGINE_MODULES_ANDROID_ANDROID_SETTINGS_H
#define BFP_ENGINE_MODULES_ANDROID_ANDROID_SETTINGS_H
#include <QString>

namespace android
{
	namespace options
	{
	static const QString ADB_BINARY = "android/adb_binary";
	static const QString SERIAL_NUMBER = "android/serial_number";
	static const QString USE_BUNDLED_ADB = "android/use_bundled_adb";
	static const QString USE_CUSTOM_ADB = "android/use_custom_adb";
	}
	namespace fallback
	{
	static const bool USE_BUNDLED_ADB = true;
	static const bool USE_CUSTOM_ADB = false;
	}
}

#endif // BFP_ENGINE_MODULES_ANDROID_ANDROID_SETTINGS_H
