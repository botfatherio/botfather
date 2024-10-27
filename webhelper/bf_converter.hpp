#ifndef BF_CONVERTER_HPP
#define BF_CONVERTER_HPP

#include <include/cef_v8.h>
#include <include/cef_values.h>

#include <QCborValue>
#include <QVariant>

namespace BFConverter {
QJsonValue CefV8ValueToQJsonValue(const CefRefPtr<CefV8Value> &cef_v8_value);
QString CefV8ValueToCompactJsonQString(
    const CefRefPtr<CefV8Value> &cef_v8_value);
QVariant CefV8ValueToQVariant(const CefRefPtr<CefV8Value> &cef_v8_value);
CefRefPtr<CefBinaryValue> QVariantToCefBinaryValue(const QVariant &variant);
QCborValue CefV8ValueToQCborValue(const CefRefPtr<CefV8Value> &cef_v8_value);
CefRefPtr<CefBinaryValue> QCborValueToCefBinaryValue(QCborValue cbor_value);
}  // namespace BFConverter

#endif  // BF_CONVERTER_HPP
