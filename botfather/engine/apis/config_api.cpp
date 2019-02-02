#include "config_api.h"
#include <QSettings>
#include <QDebug>

QVariant ConfigAPI::getValue(const QString &setting_id) const
{
	if (bot()->configIniPath().isEmpty())
	{
		return QVariant();
	}
	QSettings config(bot()->configIniPath(), QSettings::IniFormat);
	return config.value(setting_id); // FIXME: how to get the fallback? - save it from the gui
}
