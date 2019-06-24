#ifndef BROWSER_UTIL_H
#define BROWSER_UTIL_H

#include <QThread>
#include <QTimer>
#include <QApplication>
#include <QByteArray>
#include "include/cef_values.h"

namespace BrowserUtil {

void runInMainThread(std::function<void()> callback);

QByteArray convertCefBinaryValueToQByteArray(const CefRefPtr<CefBinaryValue> &cef_binary_value);

}

#endif // BROWSER_UTIL_H
