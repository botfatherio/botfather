#ifndef QT_KEYMAP_H
#define QT_KEYMAP_H

#include <QHash>
#include "qt_key_traits.h"

static const QHash<const Qt::Key, QtKeyTraits> QT_NUMPAD_KEYMAP = {
	{Qt::Key_0, {VK_NUMPAD0, XK_KP_0}},
	{Qt::Key_1, {VK_NUMPAD1, XK_KP_1}},
	{Qt::Key_2, {VK_NUMPAD2, XK_KP_2}},
	{Qt::Key_3, {VK_NUMPAD3, XK_KP_3}},
	{Qt::Key_4, {VK_NUMPAD4, XK_KP_4}},
	{Qt::Key_5, {VK_NUMPAD5, XK_KP_5}},
	{Qt::Key_6, {VK_NUMPAD6, XK_KP_6}},
	{Qt::Key_7, {VK_NUMPAD7, XK_KP_7}},
	{Qt::Key_8, {VK_NUMPAD8, XK_KP_8}},
	{Qt::Key_9, {VK_NUMPAD9, XK_KP_9}},
	{Qt::Key_Asterisk, {VK_MULTIPLY, XK_KP_Multiply}},
	{Qt::Key_Plus, {VK_ADD, XK_KP_Add}},
	{Qt::Key_Minus, {VK_SUBTRACT, XK_KP_Subtract}},
	{Qt::Key_Period, {VK_DECIMAL, XK_KP_Decimal}},
	{Qt::Key_Slash, {VK_DIVIDE, XK_KP_Divide}},
	{Qt::Key_PageUp, {VK_PRIOR, XK_KP_Page_Up}},
	{Qt::Key_PageDown, {VK_NEXT, XK_KP_Page_Down}},
	{Qt::Key_End, {VK_END, XK_KP_End}},
	{Qt::Key_Home, {VK_HOME, XK_KP_Home}},
	{Qt::Key_Left, {VK_LEFT, XK_KP_Left}},
	{Qt::Key_Up, {VK_UP, XK_KP_Up}},
	{Qt::Key_Right, {VK_RIGHT, XK_KP_Right}},
	{Qt::Key_Down, {VK_DOWN, XK_KP_Down}},
	{Qt::Key_Enter, {VK_RETURN, XK_KP_Enter}},
	{Qt::Key_Return, {VK_RETURN, XK_KP_Enter}},
	{Qt::Key_Insert, {VK_INSERT, XK_KP_Insert}},
	{Qt::Key_Delete, {VK_DELETE, XK_KP_Delete}},
};

static const QHash<const Qt::Key, QtKeyTraits> QT_NORMAL_KEYMAP = {
	{Qt::Key_Backspace, {VK_BACK, XK_BackSpace}},
	{Qt::Key_Backtab, {VK_TAB, XK_Tab}},
	{Qt::Key_Tab, {VK_TAB, XK_Tab}},
	{Qt::Key_Clear, {VK_CLEAR, XK_Clear}},
	{Qt::Key_Enter, {VK_RETURN, XK_Return}},
	{Qt::Key_Return, {VK_RETURN, XK_Return}},
	{Qt::Key_Shift, {VK_SHIFT, XK_Shift_L}},
	{Qt::Key_Control, {VK_CONTROL, XK_Control_L}},
	{Qt::Key_Menu, {VK_MENU, XK_Menu}},
	{Qt::Key_Alt, {VK_MENU, XK_Alt_L}},

	{Qt::Key_F1, {VK_F1, XK_F1}},
	{Qt::Key_F2, {VK_F2, XK_F2}},
	{Qt::Key_F3, {VK_F3, XK_F3}},
	{Qt::Key_F4, {VK_F4, XK_F4}},
	{Qt::Key_F5, {VK_F5, XK_F5}},
	{Qt::Key_F6, {VK_F6, XK_F6}},
	{Qt::Key_F7, {VK_F7, XK_F7}},
	{Qt::Key_F8, {VK_F8, XK_F8}},
	{Qt::Key_F9, {VK_F9, XK_F9}},
	{Qt::Key_F10, {VK_F10, XK_F10}},
	{Qt::Key_F11, {VK_F11, XK_F11}},
	{Qt::Key_F12, {VK_F12, XK_F12}},
	{Qt::Key_F13, {VK_F13, XK_F13}},
	{Qt::Key_F14, {VK_F14, XK_F14}},
	{Qt::Key_F15, {VK_F15, XK_F15}},
	{Qt::Key_F16, {VK_F16, XK_F16}},
	{Qt::Key_F17, {VK_F17, XK_F17}},
	{Qt::Key_F18, {VK_F18, XK_F18}},
	{Qt::Key_F19, {VK_F19, XK_F19}},
	{Qt::Key_F20, {VK_F20, XK_F20}},
	{Qt::Key_F21, {VK_F21, XK_F21}},
	{Qt::Key_F22, {VK_F22, XK_F22}},
	{Qt::Key_F23, {VK_F23, XK_F23}},
	{Qt::Key_F24, {VK_F24, XK_F24}},

	{Qt::Key_Pause, {VK_PAUSE, XK_Pause}},
	{Qt::Key_CapsLock, {VK_CAPITAL, XK_Caps_Lock}},
	{Qt::Key_Kana_Lock, {VK_KANA, XK_Kana_Lock}},
	{Qt::Key_Kana_Shift, {VK_KANA, XK_Kana_Shift}},
	{Qt::Key_Hangul, {VK_HANGUL, XK_Hangul}},
	{Qt::Key_Hangul_Hanja, {VK_HANJA, XK_Hangul_Hanja}},
	{Qt::Key_Kanji, {VK_KANJI, XK_Kanji}},
	{Qt::Key_Escape, {VK_ESCAPE, XK_Escape}},
	{Qt::Key_Space, {VK_SPACE, XK_space}},
	{Qt::Key_PageUp, {VK_PRIOR, XK_Page_Up}},
	{Qt::Key_PageDown, {VK_NEXT, XK_Page_Down}},
	{Qt::Key_End, {VK_END, XK_End}},
	{Qt::Key_Home, {VK_HOME, XK_Home}},
	{Qt::Key_Left, {VK_LEFT, XK_Left}},
	{Qt::Key_Up, {VK_UP, XK_Up}},
	{Qt::Key_Right, {VK_RIGHT, XK_Right}},
	{Qt::Key_Down, {VK_DOWN, XK_Down}},
	{Qt::Key_Select, {VK_SELECT, XK_Select}},
	{Qt::Key_Print, {VK_SNAPSHOT, XK_Print}},
	{Qt::Key_Execute, {VK_EXECUTE, XK_Execute}},
	{Qt::Key_Insert, {VK_INSERT, XK_Insert}},
	{Qt::Key_Delete, {VK_DELETE, XK_Delete}},
	{Qt::Key_Help, {VK_HELP, XK_Help}},

	{Qt::Key_0, {VK_0, XK_0}},
	{Qt::Key_ParenLeft, {VK_0, XK_parenleft}},
	{Qt::Key_1, {VK_1, XK_1}},
	{Qt::Key_2, {VK_2, XK_2}},
	{Qt::Key_At, {VK_2, XK_2}},
	{Qt::Key_3, {VK_3, XK_3}},
	{Qt::Key_NumberSign, {VK_3, XK_numbersign}},
	{Qt::Key_4, {VK_4, XK_4}},
	{Qt::Key_Dollar, {VK_4, XK_dollar}},
	{Qt::Key_5, {VK_5, XK_5}},
	{Qt::Key_Percent, {VK_5, XK_percent}},
	{Qt::Key_6, {VK_6, XK_6}},
	{Qt::Key_AsciiCircum, {VK_6, XK_asciicircum}},
	{Qt::Key_7, {VK_7, XK_7}},
	{Qt::Key_Ampersand, {VK_7, XK_ampersand}},
	{Qt::Key_8, {VK_8, XK_8}},
	{Qt::Key_Asterisk, {VK_8, XK_asterisk}},
	{Qt::Key_9, {VK_9, XK_9}},
	{Qt::Key_ParenRight, {VK_9, XK_parenright}},

	{Qt::Key_A, {VK_A, XK_A}},
	{Qt::Key_B, {VK_B, XK_B}},
	{Qt::Key_C, {VK_C, XK_C}},
	{Qt::Key_D, {VK_D, XK_D}},
	{Qt::Key_E, {VK_E, XK_E}},
	{Qt::Key_F, {VK_F, XK_F}},
	{Qt::Key_G, {VK_G, XK_G}},
	{Qt::Key_H, {VK_H, XK_H}},
	{Qt::Key_I, {VK_I, XK_I}},
	{Qt::Key_J, {VK_J, XK_J}},
	{Qt::Key_K, {VK_K, XK_K}},
	{Qt::Key_L, {VK_L, XK_L}},
	{Qt::Key_M, {VK_M, XK_M}},
	{Qt::Key_N, {VK_N, XK_N}},
	{Qt::Key_O, {VK_O, XK_O}},
	{Qt::Key_P, {VK_P, XK_P}},
	{Qt::Key_Q, {VK_Q, XK_Q}},
	{Qt::Key_R, {VK_R, XK_R}},
	{Qt::Key_S, {VK_S, XK_S}},
	{Qt::Key_T, {VK_T, XK_T}},
	{Qt::Key_U, {VK_U, XK_U}},
	{Qt::Key_V, {VK_V, XK_V}},
	{Qt::Key_W, {VK_W, XK_W}},
	{Qt::Key_X, {VK_X, XK_X}},
	{Qt::Key_Y, {VK_Y, XK_Y}},
	{Qt::Key_Z, {VK_Z, XK_Z}},

	{Qt::Key_Meta, {VK_LWIN, XK_Meta_L}},
	{Qt::Key_NumLock, {VK_NUMLOCK, XK_Num_Lock}},
	{Qt::Key_ScrollLock, {VK_SCROLL, XK_Scroll_Lock}},

	// VK_OEM_1 (BA) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the ';:' key
	// case ';': case ':': return 0xBA;
	{Qt::Key_Semicolon, {VK_OEM_1, XK_semicolon}},
	{Qt::Key_Colon, {VK_OEM_1, XK_colon}},

	// VK_OEM_PLUS (BB) Windows 2000/XP: For any country/region, the '+' key
	// case '=': case '+': return 0xBB;
	{Qt::Key_Plus, {VK_OEM_PLUS, XK_plus}},
	{Qt::Key_Equal, {VK_OEM_PLUS, XK_equal}},

	// VK_OEM_COMMA (BC) Windows 2000/XP: For any country/region, the ',' key
	// case ',': case '<': return 0xBC;
	{Qt::Key_Comma, {VK_OEM_COMMA, XK_comma}},
	{Qt::Key_Less, {VK_OEM_COMMA, XK_less}},

	// VK_OEM_MINUS (BD) Windows 2000/XP: For any country/region, the '-' key
	// case '-': case '_': return 0xBD;
	{Qt::Key_Minus, {VK_OEM_MINUS, XK_minus}},
	{Qt::Key_Underscore, {VK_OEM_MINUS, XK_underscore}},

	// VK_OEM_PERIOD (BE) Windows 2000/XP: For any country/region, the '.' key
	// case '.': case '>': return 0xBE;
	{Qt::Key_Period, {VK_OEM_PERIOD, XK_period}},
	{Qt::Key_Greater, {VK_OEM_PERIOD, XK_greater}},

	// VK_OEM_2 (BF) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '/?' key
	// case '/': case '?': return 0xBF;
	{Qt::Key_Slash, {VK_OEM_2, XK_slash}},
	{Qt::Key_Question, {VK_OEM_2, XK_question}},

	// VK_OEM_3 (C0) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '`~' key
	// case '`': case '~': return 0xC0;
	{Qt::Key_AsciiTilde, {VK_OEM_3, XK_asciitilde}},
	{Qt::Key_QuoteLeft, {VK_OEM_3, XK_quoteleft}},

	// VK_OEM_4 (DB) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '[{' key
	// case '[': case '{': return 0xDB;
	{Qt::Key_BracketLeft, {VK_OEM_4, XK_bracketleft}},
	{Qt::Key_BraceLeft, {VK_OEM_4, XK_braceleft}},

	// VK_OEM_5 (DC) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '\|' key
	// case '\\': case '|': return 0xDC;
	{Qt::Key_Backslash, {VK_OEM_5, XK_backslash}},
	{Qt::Key_Bar, {VK_OEM_5, XK_bar}},

	// VK_OEM_6 (DD) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the ']}' key
	// case ']': case '}': return 0xDD;
	{Qt::Key_BracketRight, {VK_OEM_6, XK_bracketright}},
	{Qt::Key_BraceRight, {VK_OEM_6, XK_braceright}},

	// VK_OEM_7 (DE) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
	// case '\'': case '"': return 0xDE;
	{Qt::Key_QuoteDbl, {VK_OEM_7, XK_quotedbl}}
};

#endif // QT_KEYMAP_H
