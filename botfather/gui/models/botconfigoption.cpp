#include "botconfigoption.hpp"
#include <QSettings>
#include "../../shared/qsettingsjsonformat.hpp"

QVariant BotConfigOption::value() const
{
	QSettings config(configPath(), QSettingsJsonFormat::format());
	return config.value(id(), fallback());
}

void BotConfigOption::setValue(const QVariant &new_value)
{
	QSettings config(configPath(), QSettingsJsonFormat::format());
	config.setValue(id(), new_value);
}

bool BotConfigOption::isValid() const
{
	return !id().isEmpty();
}

bool BotConfigOption::isStored() const
{
	QSettings config(configPath(), QSettingsJsonFormat::format());

	// Don't retrive the value using BotConfigSetting::value as it provides a fallback
	QVariant retrived_value = config.value(id(), QVariant());

	return !retrived_value.isNull() && retrived_value.isValid();
}

QString BotConfigOption::id() const
{
	return getStringBykey("id");
}

BotConfigOption::Type BotConfigOption::type() const
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

QString BotConfigOption::label() const
{
	return getStringBykey("label");
}

QVariant BotConfigOption::fallback() const
{
	return getVariantByKey("default");
}

QMap<QString, QString> BotConfigOption::choices() const
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

bool BotConfigOption::hasRange() const
{
	return jsonObject().contains("min") && jsonObject().contains("max") && jsonObject()["min"].isDouble() && jsonObject()["max"].isDouble();
}

int BotConfigOption::min() const
{
	return getIntegerByKey("min");
}

int BotConfigOption::max() const
{
	return getIntegerByKey("max");
}
