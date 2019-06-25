#include "config_api.hpp"
#include "../../../../shared/q_settings_json_format.hpp"
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