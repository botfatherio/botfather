#ifndef BF_SERIALIZER_HPP
#define BF_SERIALIZER_HPP

#include <QVariant>
#include "include/cef_values.h"
#include "include/cef_v8.h"

namespace BFSerializer
{
	QJsonValue CefV8ValueToQJsonValue(CefRefPtr<CefV8Value> cef_v8_value);
	QString CefV8ValueToCompactJsonQString(CefRefPtr<CefV8Value> cef_v8_value);
	QVariant CefV8ValueToQVariant(CefRefPtr<CefV8Value> cef_v8_value);
}

#endif // BF_SERIALIZER_HPP
