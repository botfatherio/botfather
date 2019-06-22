#ifndef BF_DEBUG_TOOLS_HPP
#define BF_DEBUG_TOOLS_HPP

#include <QString>
#include "include/cef_app.h"

namespace BFDebugTools
{
	// Sends a debug message to the browser process, where it will be printed to the console.
	void debug(CefRefPtr<CefBrowser> browser, const QString &message);
}

#endif // BF_DEBUG_TOOLS_HPP
