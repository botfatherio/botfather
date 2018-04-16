#ifndef BFP__SHARED__CONSTANTS_H
#define BFP__SHARED__CONSTANTS_H

#include <QSettings>
#include <QString>

namespace options
{
	namespace browser
	{
		static const QString WIDTH = "browser/width";
		static const QString HEIGHT = "browser/height";
		static const QString FLASH_SO = "browser/flash_so";
		static const QString FLASH_MANIFEST = "browser/flash_manifest";
		static const QString USE_SYSTEM_FLASH = "browser/use_system_flash";
	}
	namespace android
	{
		static const QString ADB_BINARY = "android/adb_binary";
		static const QString SERIAL_NUMBER = "android/serial_number";
	}
	namespace auth {
		static const QString USERNAME = "auth/username";
		static const QString PASSWORD = "auth/password";
		static const QString REMEMBER_ME = "auth/remember_me";
	}
}

namespace fallback
{
	namespace browser
	{
		static const int WIDTH = 1366;
		static const int HEIGHT = 768;
		static const bool USE_SYSTEM_FLASH = false;
	}
}

#endif // BFP__SHARED__CONSTANTS_H
