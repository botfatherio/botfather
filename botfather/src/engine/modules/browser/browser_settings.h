#ifndef BFP_ENGINE_MODULES_BROWSER_BROWSER_SETTINGS_H
#define BFP_ENGINE_MODULES_BROWSER_BROWSER_SETTINGS_H

#include <QString>

namespace browser
{
	namespace options {
		static const QString WIDTH = "browser/width";
		static const QString HEIGHT = "browser/height";
		static const QString FLASH_SO = "browser/flash_so";
		static const QString FLASH_MANIFEST = "browser/flash_manifest";
		static const QString USE_SYSTEM_FLASH = "browser/use_system_flash";
	}
	namespace fallback
	{
		static const int WIDTH = 1366;
		static const int HEIGHT = 768;
		static const bool USE_SYSTEM_FLASH = false;
	}
}

#endif // BFP_ENGINE_MODULES_BROWSER_BROWSER_SETTINGS_H
