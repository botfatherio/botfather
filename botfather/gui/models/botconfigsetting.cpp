#include "botconfigsetting.h"
#include <QSettings>

QVariant BotConfigSetting::value() const
{
	QSettings config(configPath(), QSettings::IniFormat);
	return config.value(id(), fallback());
}

void BotConfigSetting::setValue(const QVariant &new_value)
{
	QSettings config(configPath(), QSettings::IniFormat);
	config.setValue(id(), new_value);
}

bool BotConfigSetting::isValid() const
{
	return !id().isEmpty();
}

bool BotConfigSetting::isStored() const
{
	QSettings config(configPath(), QSettings::IniFormat);

	// Don't retrive the value using BotConfigSetting::value as it provides a fallback
	QVariant retrived_value = config.value(id(), QVariant());

	return !retrived_value.isNull() && retrived_value.isValid();
}

QString BotConfigSetting::id() const
{
	return getStringBykey("id");
}

BotConfigSetting::Type BotConfigSetting::type() const
{
	QString type_string(getStringBykey("type").toLower());
	if (type_string == "boolean")
	{
		return Type::Boolean;
	}
	if (type_string == "integer")
	{
		return Type::Integer;
	}
	if (type_string == "number")
	{
		return Type::Number;
	}
	if (type_string == "password")
	{
		return Type::Password;
	}
	if (type_string == "string")
	{
		return Type::String;
	}
	if (type_string == "choice")
	{
		return Type::Choice;
	}
	return Type::Unknown;
}

QString BotConfigSetting::label() const
{
	return getStringBykey("label");
}

QVariant BotConfigSetting::fallback() const
{
	return getVariantByKey("default");
}

QMap<QString, QString> BotConfigSetting::choices() const
{
	QMap<QString, QString> choices;

	if (!jsonObject().contains("choices") || !jsonObject()["choices"].isObject() || jsonObject()["choices"].isArray())
	{
		Q_ASSERT(choices.isEmpty());
		return choices;
	}

	QJsonObject options_object = jsonObject()["choices"].toObject();

	for (const QString &key : options_object.keys())
	{
		QJsonValue value = options_object.value(key);
		if (!value.isString())
		{
			continue;
		}
		choices[key] = value.toString();
	}

	return choices;
}

bool BotConfigSetting::hasRange() const
{
	return jsonObject().contains("min") && jsonObject().contains("max") && jsonObject()["min"].isDouble() && jsonObject()["max"].isDouble();
}

int BotConfigSetting::min() const
{
	return getIntegerByKey("min");
}

int BotConfigSetting::max() const
{
	return getIntegerByKey("max");
}
