#ifndef STATS_API_HPP
#define STATS_API_HPP

#include <QObject>
#include <QScriptEngine>
#include <QString>

#include "abstract_api.hpp"

class StatsAPI : public AbstractAPI {
    Q_OBJECT

   public:
    using AbstractAPI::AbstractAPI;

    Q_INVOKABLE void show(const QString &group, const QString &label,
                          const QString &value) const;
    Q_INVOKABLE void clear() const;
};

#endif  // STATS_API_HPP
