#include "config_api.hpp"

#include <QDebug>
#include <QSettings>

#include "../../shared/q_settings_json_format.hpp"

QVariant ConfigAPI::getValue(const QString &setting_id) const {
    if (bot()->configIniPath().isEmpty()) {
        return QVariant();
    }

    QSettings config(bot()->configIniPath(), QSettingsJsonFormat::format());
    QVariant value = config.value(setting_id);

    // Empty settings without a default are stored as a JSON null, which is read back as std::nullptr_t.
    // QtScript would wrap std::nullptr_t in a truthy object instead of null or undefined.
    // To avoid this, return an invalid QVariant, which scripts receive as undefined.
    if (value.userType() == QMetaType::Nullptr) {
        return QVariant();
    }

    return value;
}
