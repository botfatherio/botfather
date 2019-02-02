#include "botconfig.h"
#include <QJsonArray>

bool BotConfig::isValid() const
{
	for (BotConfigGroup group : groups())
	{
		for (BotConfigSetting setting : group.settings())
		{
			// The bot config is considered valid when there is atleast on valid setting
			if (setting.isValid()) return true;
		}
	}
	return false;
}

QVector<BotConfigGroup> BotConfig::groups() const
{
	QVector<BotConfigGroup> groups;

	if (!jsonObject().contains("groups") || !jsonObject()["groups"].isArray())
	{
		Q_ASSERT(groups.isEmpty());
		return groups;
	}

	for (QJsonValue group : jsonObject()["groups"].toArray())
	{
		if (!group.isObject() || group.isArray())
		{
			// Skip non object groups.
			continue;
		}

		Q_ASSERT(group.isObject());
		Q_ASSERT(!group.isArray());

		groups.append(BotConfigGroup(group.toObject(), configPath()));
	}

	return groups;
}
