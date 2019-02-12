#ifndef BROWSER_UTIL_H
#define BROWSER_UTIL_H

#include <QThread>
#include <QTimer>
#include <QApplication>

namespace BrowserUtil {

void runInMainThread(std::function<void()> callback);

}

#endif // BROWSER_UTIL_H
