#include "bot_config_group.hpp"

#include <QJsonArray>

QString BotConfigGroup::name() const { return getStringBykey("name"); }

QVector<BotConfigOption> BotConfigGroup::options() const {
    QVector<BotConfigOption> config_options;

    if (!jsonObject().contains("options") ||
        !jsonObject()["options"].isArray()) {
        Q_ASSERT(config_options.isEmpty());
        return config_options;
    }

    for (QJsonValue option : jsonObject()["options"].toArray()) {
        if (!option.isObject() || option.isArray()) {
            // Skip non object options.
            continue;
        }

        Q_ASSERT(option.isObject());
        Q_ASSERT(!option.isArray());

        config_options.append(BotConfigOption(option.toObject(), configPath()));
    }

    return config_options;
}
