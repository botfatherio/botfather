#ifndef BFP__SCRIPTING__GENERAL_SETTINGS_H
#define BFP__SCRIPTING__GENERAL_SETTINGS_H
#include <QString>

namespace general
{
	namespace options {
		static const QString AUTOKILL = "general/autokill";
		static const QString AUTOKILL_PERIOD = "general/autokill_period";
		static const QString STOP_SHORTCUT = "shortcuts/stop_shortcut";
		static const QString KILL_SHORTCUT = "shortcuts/kill_shortcut";
	}
	namespace fallback
	{
		static const bool AUTOKILL = false;
		static const int AUTOKILL_PERIOD = 7;
	}
}

#endif // BFP__SCRIPTING__GENERAL_SETTINGS_H
