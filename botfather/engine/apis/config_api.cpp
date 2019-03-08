#include "config_api.h"
#include "../../shared/qsettingsjsonformat.h"
#include <QSettings>
#include <QDebug>

QVariant ConfigAPI::getValue(const QString &setting_id) const
{
	if (bot()->configIniPath().isEmpty())
	{
		return QVariant();
	}
	QSettings config(bot()->configIniPath(), QSettingsJsonFormat::format());
	return config.value(setting_id);
}
