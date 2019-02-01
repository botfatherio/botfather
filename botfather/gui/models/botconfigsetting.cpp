#include "botconfigsetting.h"

bool BotConfigSetting::isValid() const
{
	return !id().isEmpty();
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
