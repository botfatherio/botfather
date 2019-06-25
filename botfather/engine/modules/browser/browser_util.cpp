#include "browser_util.hpp"
#include <QScriptEngine>
#include <QCborArray>
#include <QCborMap>

void BrowserUtil::runInMainThread(std::function<void()> callback)
{
	QTimer *timer = new QTimer;
	timer->setSingleShot(true);
	timer->moveToThread(QApplication::instance()->thread());

	QObject::connect(timer, &QTimer::timeout, [=]()
	{
		// This code will run in the main thread
		callback();
		timer->deleteLater();
	});

	QMetaObject::invokeMethod(timer, "start", Qt::QueuedConnection, Q_ARG(int, 0));
}

QByteArray BrowserUtil::convertCefBinaryValueToQByteArray(const CefRefPtr<CefBinaryValue> &cef_binary_value)
{
	size_t buffer_size = cef_binary_value->GetSize();
	char *buffer = new char[buffer_size];

	cef_binary_value->GetData(buffer, buffer_size, 0);
	QByteArray byte_array(buffer, buffer_size);

	delete[] buffer;
	return byte_array;
}

QScriptValue BrowserUtil::convertToQScriptValue(QScriptEngine *engine, const QCborValue &cbor_value)
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
			array.setProperty(i, BrowserUtil::convertToQScriptValue(engine, cbor_array[i]));
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
			object.setProperty(key.toString(), BrowserUtil::convertToQScriptValue(engine, cbor_map[key]));
		}
		return object;
	}

	return QScriptValue(QScriptValue::UndefinedValue); // Fallback
}
