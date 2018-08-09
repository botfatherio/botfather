#ifndef BFP__SCRIPTING__GENERAL_SETTINGS_H
#define BFP__SCRIPTING__GENERAL_SETTINGS_H
#include <QString>

// NOTE: One can not name a .ini file group "general", it'S a reserved name.

namespace general
{
	namespace options {
		static const QString STOP_SHORTCUT = "shortcuts/stop_shortcut";
	}
	namespace fallback
	{
	}
}

#endif // BFP__SCRIPTING__GENERAL_SETTINGS_H
