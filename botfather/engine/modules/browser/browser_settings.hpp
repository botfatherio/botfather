#ifndef BFP_ENGINE_MODULES_BROWSER_BROWSER_SETTINGS_H
#define BFP_ENGINE_MODULES_BROWSER_BROWSER_SETTINGS_H

#include <QString>
#include <QSize>

namespace browser
{
	namespace options {
		static const QString FLASH_SO = "browser/flash_so";
		static const QString FLASH_MANIFEST = "browser/flash_manifest";
		static const QString USE_SYSTEM_FLASH = "browser/use_system_flash";
		static const QString USE_BUNDLED_FLASH = "browser/use_bundled_flash";
		static const QString USE_CUSTOM_FLASH = "browser/use_custom_flash";
	}
	namespace fallback
	{
		static const bool USE_SYSTEM_FLASH = true;
		static const bool USE_BUNDLED_FLASH = true;
		static const bool USE_CUSTOM_FLASH = !USE_BUNDLED_FLASH;
		static const QString BUNDLED_FLASH_SO_PATH = "/app/extra/flash/libpepflashplayer.so";
		static const QString BUNDLED_FLASH_MANIFEST_PATH = "/app/extra/flash/manifest.json";
	}
}

#endif // BFP_ENGINE_MODULES_BROWSER_BROWSER_SETTINGS_H
