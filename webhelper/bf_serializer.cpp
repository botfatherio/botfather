#include "bf_serializer.hpp"
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QJsonValue BFSerializer::CefV8ValueToQJsonValue(CefRefPtr<CefV8Value> cef_v8_value)
{
	if (!cef_v8_value->IsValid())
	{
		return QJsonValue(QJsonValue::Undefined);
	}

	// Special

	if (cef_v8_value->IsNull())
	{
		return QJsonValue(QJsonValue::Null);
	}

	if (cef_v8_value->IsUndefined())
	{
		return QJsonValue(QJsonValue::Undefined);
	}

	// Primitive

	if (cef_v8_value->IsBool())
	{
		return QJsonValue(cef_v8_value->GetBoolValue());
	}

	if (cef_v8_value->IsDouble())
	{
		return QJsonValue(cef_v8_value->GetDoubleValue());
	}

	if (cef_v8_value->IsInt())
	{
		return QJsonValue(cef_v8_value->GetIntValue());
	}

	if (cef_v8_value->IsUInt())
	{
		return QJsonValue((qint64)cef_v8_value->GetUIntValue());
	}

	if (cef_v8_value->IsString())
	{
		return QJsonValue(QString::fromStdString(cef_v8_value->GetStringValue().ToString()));
	}

	// Complex

	if (cef_v8_value->IsDate())
	{
		/* TODO: fix datetime
		CefTime cef_time = cef_v8_value->GetDateValue();
		QDate date(cef_time.year, cef_time.month, cef_time.day_of_month);
		QTime time(cef_time.hour, cef_time.minute, cef_time.second, cef_time.millisecond);
		return QJsonValue(QDateTime(date, time));
		*/
		return QJsonValue(QString("TODO: FIX DATETIME"));
	}

	if (cef_v8_value->IsArray())
	{
		QJsonArray array;
		for (int i = 0; i < cef_v8_value->GetArrayLength(); ++i)
		{
			array.append(BFSerializer::CefV8ValueToQJsonValue(cef_v8_value->GetValue(i)));
		}
		return QJsonValue(array);
	}

	if (cef_v8_value->IsObject())
	{
		QJsonObject object;
		std::vector<CefString> keys;

		if (!cef_v8_value->GetKeys(keys))
		{
			return QJsonValue(object);
		}

		for (const CefString &key : keys)
		{
			QString q_string_key = QString::fromStdString(key.ToString());
			object.insert(q_string_key, BFSerializer::CefV8ValueToQJsonValue(cef_v8_value->GetValue(key)));
		}
		return QJsonValue(object);
	}

	if (cef_v8_value->IsArrayBuffer())
	{
		return QJsonValue(QJsonValue::Undefined);
	}

	if (cef_v8_value->IsFunction())
	{
		return QJsonValue(QString::fromStdString(cef_v8_value->GetFunctionName().ToString()));
	}

	return QJsonValue(QJsonValue::Undefined); // Fallback
}

QString BFSerializer::CefV8ValueToCompactJsonQString(CefRefPtr<CefV8Value> cef_v8_value)
{
	QJsonValue json_value(BFSerializer::CefV8ValueToQJsonValue(cef_v8_value));
	QJsonObject root_object;
	root_object.insert("ROOT", json_value);

	QJsonDocument json_document(root_object);
	return QString(json_document.toJson(QJsonDocument::Compact));
}

QVariant BFSerializer::CefV8ValueToQVariant(CefRefPtr<CefV8Value> cef_v8_value)
{
	if (!cef_v8_value->IsValid())
	{
		return QVariant(QJsonValue(QJsonValue::Undefined));
	}

	// Special

	if (cef_v8_value->IsNull())
	{
		return QVariant(QJsonValue(QJsonValue::Null));
	}

	if (cef_v8_value->IsUndefined())
	{
		return QVariant(QJsonValue(QJsonValue::Undefined));
	}

	// Primitive

	if (cef_v8_value->IsBool())
	{
		return QVariant(cef_v8_value->GetBoolValue());
	}

	if (cef_v8_value->IsDouble())
	{
		return QVariant(cef_v8_value->GetDoubleValue());
	}

	if (cef_v8_value->IsInt())
	{
		return QVariant(cef_v8_value->GetIntValue());
	}

	if (cef_v8_value->IsUInt())
	{
		return QVariant(cef_v8_value->GetUIntValue());
	}

	if (cef_v8_value->IsString())
	{
		return QVariant(QString::fromStdString(cef_v8_value->GetStringValue().ToString()));
	}

	// Complex

	if (cef_v8_value->IsDate())
	{
		CefTime cef_time = cef_v8_value->GetDateValue();
		QDate date(cef_time.year, cef_time.month, cef_time.day_of_month);
		QTime time(cef_time.hour, cef_time.minute, cef_time.second, cef_time.millisecond);
		return QVariant(QDateTime(date, time));
	}

	if (cef_v8_value->IsArray())
	{
		QVariantList array;
		for (int i = 0; i < cef_v8_value->GetArrayLength(); ++i)
		{
			array.append(BFSerializer::CefV8ValueToQJsonValue(cef_v8_value->GetValue(i)));
		}
		return array;
	}

	if (cef_v8_value->IsObject())
	{
		QVariantMap object;
		std::vector<CefString> keys;

		if (!cef_v8_value->GetKeys(keys))
		{
			return object;
		}

		for (const CefString &key : keys)
		{
			QString q_string_key = QString::fromStdString(key.ToString());
			object.insert(q_string_key, BFSerializer::CefV8ValueToQVariant(cef_v8_value->GetValue(key)));
		}
		return object;
	}

	if (cef_v8_value->IsArrayBuffer())
	{
		return QVariant(QJsonValue(QJsonValue::Undefined));
	}

	if (cef_v8_value->IsFunction())
	{
		return QVariant(QString::fromStdString(cef_v8_value->GetFunctionName().ToString()));
	}

	return QVariant(QJsonValue(QJsonValue::Undefined)); // Fallback
}
