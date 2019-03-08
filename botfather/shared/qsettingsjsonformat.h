#ifndef QSETTINGSJSONFORMAT_H
#define QSETTINGSJSONFORMAT_H

#include <QSettings>

namespace QSettingsJsonFormat {

QSettings::Format format();
void setFormat(const QSettings::Format &format);
bool readFunc(QIODevice &device, QSettings::SettingsMap &map);
bool writeFunc(QIODevice &device, const QSettings::SettingsMap &map);

}

#endif // QSETTINGSJSONFORMAT_H
