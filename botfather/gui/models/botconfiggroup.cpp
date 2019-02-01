#include "botconfiggroup.h"
#include <QJsonArray>

QString BotConfigGroup::name() const
{
	return getStringBykey("name");
}

QVector<BotConfigSetting> BotConfigGroup::settings() const
{
	QVector<BotConfigSetting> config_settings;

	if (!jsonObject().contains("settings") || !jsonObject()["settings"].isArray())
	{
		Q_ASSERT(config_settings.isEmpty());
		return config_settings;
	}

	for (QJsonValue setting : jsonObject()["settings"].toArray())
	{
		if (!setting.isObject() || setting.isArray())
		{
			// Skip non object options.
			continue;
		}

		Q_ASSERT(setting.isObject());
		Q_ASSERT(!setting.isArray());

		config_settings.append(BotConfigSetting(setting.toObject(), configPath()));
	}

	return config_settings;
}
