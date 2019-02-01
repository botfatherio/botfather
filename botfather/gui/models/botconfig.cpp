#include "botconfig.h"
#include <QJsonArray>

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

		groups.append(BotConfigGroup(group.toObject()));
	}

	return groups;
}
