#ifndef CONFIG_API_H
#define CONFIG_API_H

#include <QObject>
#include <QScriptEngine>
#include <QString>

#include "abstract_api.hpp"

class ConfigAPI : public AbstractAPI {
    Q_OBJECT

   public:
    using AbstractAPI::AbstractAPI;

    Q_INVOKABLE QVariant getValue(const QString &setting_id) const;
};

#endif  // CONFIG_API_H
