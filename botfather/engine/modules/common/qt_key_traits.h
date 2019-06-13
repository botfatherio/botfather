#ifndef QT_KEY_TRAITS_H
#define QT_KEY_TRAITS_H

#include "windows_keyboard_codes.h"
#include "linux11_keyboard_codes.h"

struct QtKeyTraits
{
	int windows_key_code = VK_UNKNOWN;
	int linux11_key_code = XK_VoidSymbol;

	QtKeyTraits(int windows_key_code, int linux11_key_code)
		: windows_key_code(windows_key_code), linux11_key_code(linux11_key_code)
	{}
};

#endif // QT_KEY_TRAITS_H
