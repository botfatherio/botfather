#include "stats_api.hpp"

void StatsAPI::setValue(const QString &group, const QString &label, const QString &value) const
{
        emit bot()->statsUpdated(group, label, value);
}
