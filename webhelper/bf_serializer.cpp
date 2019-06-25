#include "bf_serializer.hpp"
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDataStream>
#include <QCborArray>
#include <QCborMap>

QJsonValue BFSerializer::CefV8ValueToQJsonValue(const CefRefPtr<CefV8Value> &cef_v8_value)
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

QString BFSerializer::CefV8ValueToCompactJsonQString(const CefRefPtr<CefV8Value> &cef_v8_value)
{
	QJsonValue json_value(BFSerializer::CefV8ValueToQJsonValue(cef_v8_value));
	QJsonObject root_object;
	root_object.insert("ROOT", json_value);

	QJsonDocument json_document(root_object);
	return QString(json_document.toJson(QJsonDocument::Compact));
}

QVariant BFSerializer::CefV8ValueToQVariant(const CefRefPtr<CefV8Value> &cef_v8_value)
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

CefRefPtr<CefBinaryValue> BFSerializer::QVariantToCefBinaryValue(const QVariant &variant)
{
	QByteArray byte_array;
	QDataStream data_stream(&byte_array, QIODevice::WriteOnly);

	data_stream << variant;

	const void* cvp = static_cast<const void*>(byte_array.data());
	return CefBinaryValue::Create(cvp, (size_t)byte_array.length());
}

QCborValue BFSerializer::CefV8ValueToQCborValue(const CefRefPtr<CefV8Value> &cef_v8_value)
{
	if (!cef_v8_value->IsValid())
	{
		return QCborValue(QCborValue::Invalid);
	}

	// Special

	if (cef_v8_value->IsNull())
	{
		return QCborValue(QCborValue::Null);
	}

	if (cef_v8_value->IsUndefined())
	{
		return QCborValue(QCborValue::Undefined);
	}

	// Primitive

	if (cef_v8_value->IsBool())
	{
		return QCborValue(cef_v8_value->GetBoolValue());
	}

	if (cef_v8_value->IsDouble())
	{
		return QCborValue(cef_v8_value->GetDoubleValue());
	}

	if (cef_v8_value->IsInt())
	{
		return QCborValue(cef_v8_value->GetIntValue());
	}

	if (cef_v8_value->IsUInt())
	{
		return QCborValue(cef_v8_value->GetUIntValue());
	}

	if (cef_v8_value->IsString())
	{
		return QCborValue(QString::fromStdString(cef_v8_value->GetStringValue().ToString()));
	}

	// Complex

	if (cef_v8_value->IsDate())
	{
		CefTime cef_time = cef_v8_value->GetDateValue();
		QDate date(cef_time.year, cef_time.month, cef_time.day_of_month);
		QTime time(cef_time.hour, cef_time.minute, cef_time.second, cef_time.millisecond);
		return QCborValue(QDateTime(date, time));
	}

	if (cef_v8_value->IsArray())
	{
		QCborArray array;
		for (int i = 0; i < cef_v8_value->GetArrayLength(); ++i)
		{
			array.append(BFSerializer::CefV8ValueToQCborValue(cef_v8_value->GetValue(i)));
		}
		return array;
	}

	if (cef_v8_value->IsObject())
	{
		QCborMap object;
		std::vector<CefString> keys;

		if (!cef_v8_value->GetKeys(keys))
		{
			return object;
		}

		for (const CefString &key : keys)
		{
			QString q_string_key = QString::fromStdString(key.ToString());
			object.insert(q_string_key, BFSerializer::CefV8ValueToQCborValue(cef_v8_value->GetValue(key)));
		}
		return object;
	}

	if (cef_v8_value->IsArrayBuffer())
	{
		return QCborValue(QCborValue::Undefined);
	}

	if (cef_v8_value->IsFunction())
	{
		return QCborValue(QString::fromStdString(cef_v8_value->GetFunctionName().ToString()));
	}

	return QCborValue(QCborValue::Undefined); // Fallback
}

CefRefPtr<CefBinaryValue> BFSerializer::QCborValueToCefBinaryValue(QCborValue cbor_value)
{
	QByteArray byte_array = cbor_value.toCbor();
	const void* cvp = static_cast<const void*>(byte_array.data());
	return CefBinaryValue::Create(cvp, (size_t)byte_array.length());
}
