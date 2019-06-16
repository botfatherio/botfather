#include "q_settings_json_format.hpp"
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>

namespace
{
	static QSettings::Format m_format = QSettings::InvalidFormat;
}

QSettings::Format QSettingsJsonFormat::format()
{
	return m_format;
}

void QSettingsJsonFormat::setFormat(const QSettings::Format &format)
{
	m_format = format;
}

bool QSettingsJsonFormat::readFunc(QIODevice &device, QSettings::SettingsMap &map)
{
	QString json_text = device.readAll();
	// Don't close the device, causes crash.

	QJsonParseError parse_error;
	QJsonDocument json_document = QJsonDocument::fromJson(json_text.toUtf8(), &parse_error);
	QJsonObject json_object = json_document.object();

	map = json_object.toVariantMap();
	return parse_error.error == QJsonParseError::NoError;
}

bool QSettingsJsonFormat::writeFunc(QIODevice &device, const QSettings::SettingsMap &map)
{
	QJsonObject json_object = QJsonObject::fromVariantMap(map);
	QJsonDocument json_document(json_object);
	QByteArray json_text = json_document.toJson();

	// Don't close the device, causes crash.
	return device.write(json_text) != -1; // return code is -1 if an error occurred
}
