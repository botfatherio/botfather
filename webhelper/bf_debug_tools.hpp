#ifndef BF_DEBUG_TOOLS_HPP
#define BF_DEBUG_TOOLS_HPP

#include <include/cef_app.h>

#include <QString>

namespace BFDebugTools {
// Sends a debug message to the browser process, where it will be printed to the
// console.
void debug(CefRefPtr<CefBrowser> browser, const QString &message);
}  // namespace BFDebugTools

#endif  // BF_DEBUG_TOOLS_HPP
