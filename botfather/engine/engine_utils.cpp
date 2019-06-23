#include "engine_utils.hpp"
#include <QScriptEngine>
#include <QCborArray>
#include <QCborMap>

QScriptValue EngineUtils::convertToQScriptValue(QScriptEngine *engine, const QCborValue &cbor_value)
{
	if (cbor_value.isInvalid())
	{
		return QScriptValue(QScriptValue::NullValue);
	}

	// Special

	if (cbor_value.isNull())
	{
		return QScriptValue(QScriptValue::NullValue);
	}

	if (cbor_value.isUndefined())
	{
		return QScriptValue(QScriptValue::UndefinedValue);
	}

	// Primitive

	if (cbor_value.isBool())
	{
		return QScriptValue(cbor_value.toBool());
	}

	if (cbor_value.isDouble())
	{
		return QScriptValue(cbor_value.toDouble());
	}

	if (cbor_value.isInteger())
	{
		// QScriptValue(qint64) is ambigious
		return QScriptValue((double)cbor_value.toInteger());
	}

	if (cbor_value.isString())
	{
		return QScriptValue(cbor_value.toString());
	}

	// Complex

	if (cbor_value.isDateTime())
	{
		return engine->newDate(cbor_value.toDateTime().toMSecsSinceEpoch());
	}

	if (cbor_value.isArray())
	{
		QCborArray cbor_array = cbor_value.toArray();
		QScriptValue array = engine->newArray(cbor_array.size());

		for (int i = 0; i < cbor_array.size(); ++i)
		{
			array.setProperty(i, EngineUtils::convertToQScriptValue(engine, cbor_array[i]));
		}
		return array;
	}

	if (cbor_value.isMap())
	{
		QCborMap cbor_map = cbor_value.toMap();
		QScriptValue object = engine->newObject();

		for (const QCborValue key : cbor_map.keys())
		{
			if (!key.isString()) continue;
			object.setProperty(key.toString(), EngineUtils::convertToQScriptValue(engine, cbor_map[key]));
		}
		return object;
	}

	return QScriptValue(QScriptValue::UndefinedValue); // Fallback
}
