#include "config_api.hpp"
#include "../../../../shared/qsettingsjsonformat.hpp"
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
