#include "bf_key_mapper.h"
#include <QHash>
#include "windows_keyboard_codes.h"
#include "linux11_keyboard_codes.h"
#include "include/cef_base.h"
#include "bf_keymap.h"

Qt::KeyboardModifier BFKeyMapper::mapBFKeycodeToQtKeyboardModifier(const QString &bf_keycode)
{
	const QString upper_keycode(bf_keycode.toUpper());
	if (BF_KEYMAP.contains(upper_keycode))
	{
		return BF_KEYMAP[upper_keycode].qt_keyboard_modifier;
	}
	return Qt::NoModifier;
}

Qt::Key BFKeyMapper::mapBFKeycodeToQtKey(const QString &bf_keycode)
{
	const QString upper_keycode(bf_keycode.toUpper());
	if (BF_KEYMAP.contains(upper_keycode))
	{
		return BF_KEYMAP[upper_keycode].qt_key;
	}
	return Qt::Key();
}

QChar BFKeyMapper::mapBFKeycodeToQChar(const QString &bf_keycode)
{
	if (BF_KEYMAP.contains(bf_keycode))
	{
		return BF_KEYMAP[bf_keycode].q_char;
	}
	return QChar();
}

int BFKeyMapper::mapQtKeyToWindowsKeyCode(const Qt::Key &qt_key, bool is_keypad_key)
{
	if (is_keypad_key)
	{
		switch (qt_key)
		{
		case Qt::Key_0:
			return VK_NUMPAD0; // (60) Numeric keypad 0 key
		case Qt::Key_1:
			return VK_NUMPAD1; // (61) Numeric keypad 1 key
		case Qt::Key_2:
			return  VK_NUMPAD2; // (62) Numeric keypad 2 key
		case Qt::Key_3:
			return VK_NUMPAD3; // (63) Numeric keypad 3 key
		case Qt::Key_4:
			return VK_NUMPAD4; // (64) Numeric keypad 4 key
		case Qt::Key_5:
			return VK_NUMPAD5; // (65) Numeric keypad 5 key
		case Qt::Key_6:
			return VK_NUMPAD6; // (66) Numeric keypad 6 key
		case Qt::Key_7:
			return VK_NUMPAD7; // (67) Numeric keypad 7 key
		case Qt::Key_8:
			return VK_NUMPAD8; // (68) Numeric keypad 8 key
		case Qt::Key_9:
			return VK_NUMPAD9; // (69) Numeric keypad 9 key
		case Qt::Key_Asterisk:
			return VK_MULTIPLY; // (6A) Multiply key
		case Qt::Key_Plus:
			return VK_ADD; // (6B) Add key
		case Qt::Key_Minus:
			return VK_SUBTRACT; // (6D) Subtract key
		case Qt::Key_Period:
			return VK_DECIMAL; // (6E) Decimal key
		case Qt::Key_Slash:
			return VK_DIVIDE; // (6F) Divide key
		case Qt::Key_PageUp:
			return VK_PRIOR; // (21) PAGE UP key
		case Qt::Key_PageDown:
			return VK_NEXT; // (22) PAGE DOWN key
		case Qt::Key_End:
			return VK_END; // (23) END key
		case Qt::Key_Home:
			return VK_HOME; // (24) HOME key
		case Qt::Key_Left:
			return VK_LEFT; // (25) LEFT ARROW key
		case Qt::Key_Up:
			return VK_UP; // (26) UP ARROW key
		case Qt::Key_Right:
			return VK_RIGHT; // (27) RIGHT ARROW key
		case Qt::Key_Down:
			return VK_DOWN; // (28) DOWN ARROW key
		case Qt::Key_Enter:
		case Qt::Key_Return:
			return VK_RETURN; // (0D) Return key
		case Qt::Key_Insert:
			return VK_INSERT; // (2D) INS key
		case Qt::Key_Delete:
			return VK_DELETE; // (2E) DEL key
		default:
			return 0;
		}
	}

	switch (qt_key)
	{
	case Qt::Key_Backspace:
		return VK_BACK; // (08) BACKSPACE key
	case Qt::Key_Backtab:
	case Qt::Key_Tab:
		return VK_TAB; // (09) TAB key
	case Qt::Key_Clear:
		return VK_CLEAR; // (0C) CLEAR key
	case Qt::Key_Enter:
	case Qt::Key_Return:
		return VK_RETURN; // (0D) Return key
	case Qt::Key_Shift:
		return VK_SHIFT; // (10) SHIFT key
	case Qt::Key_Control:
		return VK_CONTROL; // (11) CTRL key
	case Qt::Key_Menu:
	case Qt::Key_Alt:
		return VK_MENU; // (12) ALT key

	case Qt::Key_F1:
		return VK_F1;
	case Qt::Key_F2:
		return VK_F2;
	case Qt::Key_F3:
		return VK_F3;
	case Qt::Key_F4:
		return VK_F4;
	case Qt::Key_F5:
		return VK_F5;
	case Qt::Key_F6:
		return VK_F6;
	case Qt::Key_F7:
		return VK_F7;
	case Qt::Key_F8:
		return VK_F8;
	case Qt::Key_F9:
		return VK_F9;
	case Qt::Key_F10:
		return VK_F10;
	case Qt::Key_F11:
		return VK_F11;
	case Qt::Key_F12:
		return VK_F12;
	case Qt::Key_F13:
		return VK_F13;
	case Qt::Key_F14:
		return VK_F14;
	case Qt::Key_F15:
		return VK_F15;
	case Qt::Key_F16:
		return VK_F16;
	case Qt::Key_F17:
		return VK_F17;
	case Qt::Key_F18:
		return VK_F18;
	case Qt::Key_F19:
		return VK_F19;
	case Qt::Key_F20:
		return VK_F20;
	case Qt::Key_F21:
		return VK_F21;
	case Qt::Key_F22:
		return VK_F22;
	case Qt::Key_F23:
		return VK_F23;
	case Qt::Key_F24:
		return VK_F24;

	case Qt::Key_Pause:
		return VK_PAUSE; // (13) PAUSE key
	case Qt::Key_CapsLock:
		return VK_CAPITAL; // (14) CAPS LOCK key
	case Qt::Key_Kana_Lock:
	case Qt::Key_Kana_Shift:
		return VK_KANA; // (15) Input Method Editor (IME) Kana mode
	case Qt::Key_Hangul:
		return VK_HANGUL; // VK_HANGUL (15) IME Hangul mode
		// VK_JUNJA (17) IME Junja mode
		// VK_FINAL (18) IME final mode
	case Qt::Key_Hangul_Hanja:
		return VK_HANJA; // (19) IME Hanja mode
	case Qt::Key_Kanji:
		return VK_KANJI; // (19) IME Kanji mode
	case Qt::Key_Escape:
		return VK_ESCAPE; // (1B) ESC key
		// VK_CONVERT (1C) IME convert
		// VK_NONCONVERT (1D) IME nonconvert
		// VK_ACCEPT (1E) IME accept
		// VK_MODECHANGE (1F) IME mode change request
	case Qt::Key_Space:
		return VK_SPACE; // (20) SPACEBAR
	case Qt::Key_PageUp:
		return VK_PRIOR; // (21) PAGE UP key
	case Qt::Key_PageDown:
		return VK_NEXT; // (22) PAGE DOWN key
	case Qt::Key_End:
		return VK_END; // (23) END key
	case Qt::Key_Home:
		return VK_HOME; // (24) HOME key
	case Qt::Key_Left:
		return VK_LEFT; // (25) LEFT ARROW key
	case Qt::Key_Up:
		return VK_UP; // (26) UP ARROW key
	case Qt::Key_Right:
		return VK_RIGHT; // (27) RIGHT ARROW key
	case Qt::Key_Down:
		return VK_DOWN; // (28) DOWN ARROW key
	case Qt::Key_Select:
		return VK_SELECT; // (29) SELECT key
	case Qt::Key_Print:
		return VK_SNAPSHOT; // (2A) PRINT key
	case Qt::Key_Execute:
		return VK_EXECUTE; // (2B) EXECUTE key
	case Qt::Key_Insert:
		return VK_INSERT; // (2D) INS key
	case Qt::Key_Delete:
		return VK_DELETE; // (2E) DEL key
	case Qt::Key_Help:
		return VK_HELP; // (2F) HELP key
	case Qt::Key_0:
	case Qt::Key_ParenLeft:
		return VK_0; // (30) 0) key
	case Qt::Key_1:
		return VK_1; // (31) 1 ! key
	case Qt::Key_2:
	case Qt::Key_At:
		return VK_2; // (32) 2 & key
	case Qt::Key_3:
	case Qt::Key_NumberSign:
		return VK_3; // case '3': case '#';
	case Qt::Key_4:
	case Qt::Key_Dollar: // (34) 4 key '$';
		return VK_4;
	case Qt::Key_5:
	case Qt::Key_Percent:
		return VK_5; // (35) 5 key  '%'
	case Qt::Key_6:
	case Qt::Key_AsciiCircum:
		return VK_6; // (36) 6 key  '^'
	case Qt::Key_7:
	case Qt::Key_Ampersand:
		return VK_7; // (37) 7 key  case '&'
	case Qt::Key_8:
	case Qt::Key_Asterisk:
		return VK_8; // (38) 8 key  '*'
	case Qt::Key_9:
	case Qt::Key_ParenRight:
		return VK_9; // (39) 9 key '('
	case Qt::Key_A:
		return VK_A; // (41) A key case 'a': case 'A': return 0x41;
	case Qt::Key_B:
		return VK_B; // (42) B key case 'b': case 'B': return 0x42;
	case Qt::Key_C:
		return VK_C; // (43) C key case 'c': case 'C': return 0x43;
	case Qt::Key_D:
		return VK_D; // (44) D key case 'd': case 'D': return 0x44;
	case Qt::Key_E:
		return VK_E; // (45) E key case 'e': case 'E': return 0x45;
	case Qt::Key_F:
		return VK_F; // (46) F key case 'f': case 'F': return 0x46;
	case Qt::Key_G:
		return VK_G; // (47) G key case 'g': case 'G': return 0x47;
	case Qt::Key_H:
		return VK_H; // (48) H key case 'h': case 'H': return 0x48;
	case Qt::Key_I:
		return VK_I; // (49) I key case 'i': case 'I': return 0x49;
	case Qt::Key_J:
		return VK_J; // (4A) J key case 'j': case 'J': return 0x4A;
	case Qt::Key_K:
		return VK_K; // (4B) K key case 'k': case 'K': return 0x4B;
	case Qt::Key_L:
		return VK_L; // (4C) L key case 'l': case 'L': return 0x4C;
	case Qt::Key_M:
		return VK_M; // (4D) M key case 'm': case 'M': return 0x4D;
	case Qt::Key_N:
		return VK_N; // (4E) N key case 'n': case 'N': return 0x4E;
	case Qt::Key_O:
		return VK_O; // (4F) O key case 'o': case 'O': return 0x4F;
	case Qt::Key_P:
		return VK_P; // (50) P key case 'p': case 'P': return 0x50;
	case Qt::Key_Q:
		return VK_Q; // (51) Q key case 'q': case 'Q': return 0x51;
	case Qt::Key_R:
		return VK_R; // (52) R key case 'r': case 'R': return 0x52;
	case Qt::Key_S:
		return VK_S; // (53) S key case 's': case 'S': return 0x53;
	case Qt::Key_T:
		return VK_T; // (54) T key case 't': case 'T': return 0x54;
	case Qt::Key_U:
		return VK_U; // (55) U key case 'u': case 'U': return 0x55;
	case Qt::Key_V:
		return VK_V; // (56) V key case 'v': case 'V': return 0x56;
	case Qt::Key_W:
		return VK_W; // (57) W key case 'w': case 'W': return 0x57;
	case Qt::Key_X:
		return VK_X; // (58) X key case 'x': case 'X': return 0x58;
	case Qt::Key_Y:
		return VK_Y; // (59) Y key case 'y': case 'Y': return 0x59;
	case Qt::Key_Z:
		return VK_Z; // (5A) Z key case 'z': case 'Z': return 0x5A;
	case Qt::Key_Meta:
		return VK_LWIN; // (5B) Left Windows key (Microsoft Natural keyboard)
		// case Qt::Key_Meta_R: FIXME: What to do here?
		//    return VK_RWIN; // (5C) Right Windows key (Natural keyboard)
		// VK_APPS (5D) Applications key (Natural keyboard)
		// VK_SLEEP (5F) Computer Sleep key
		// VK_SEPARATOR (6C) Separator key
		// VK_SUBTRACT (6D) Subtract key
		// VK_DECIMAL (6E) Decimal key
		// VK_DIVIDE (6F) Divide key
		// handled by key code above

	case Qt::Key_NumLock:
		return VK_NUMLOCK; // (90) NUM LOCK key

	case Qt::Key_ScrollLock:
		return VK_SCROLL; // (91) SCROLL LOCK key

		// VK_LSHIFT (A0) Left SHIFT key
		// VK_RSHIFT (A1) Right SHIFT key
		// VK_LCONTROL (A2) Left CONTROL key
		// VK_RCONTROL (A3) Right CONTROL key
		// VK_LMENU (A4) Left MENU key
		// VK_RMENU (A5) Right MENU key
		// VK_BROWSER_BACK (A6) Windows 2000/XP: Browser Back key
		// VK_BROWSER_FORWARD (A7) Windows 2000/XP: Browser Forward key
		// VK_BROWSER_REFRESH (A8) Windows 2000/XP: Browser Refresh key
		// VK_BROWSER_STOP (A9) Windows 2000/XP: Browser Stop key
		// VK_BROWSER_SEARCH (AA) Windows 2000/XP: Browser Search key
		// VK_BROWSER_FAVORITES (AB) Windows 2000/XP: Browser Favorites key
		// VK_BROWSER_HOME (AC) Windows 2000/XP: Browser Start and Home key
		// VK_VOLUME_MUTE (AD) Windows 2000/XP: Volume Mute key
		// VK_VOLUME_DOWN (AE) Windows 2000/XP: Volume Down key
		// VK_VOLUME_UP (AF) Windows 2000/XP: Volume Up key
		// VK_MEDIA_NEXT_TRACK (B0) Windows 2000/XP: Next Track key
		// VK_MEDIA_PREV_TRACK (B1) Windows 2000/XP: Previous Track key
		// VK_MEDIA_STOP (B2) Windows 2000/XP: Stop Media key
		// VK_MEDIA_PLAY_PAUSE (B3) Windows 2000/XP: Play/Pause Media key
		// VK_LAUNCH_MAIL (B4) Windows 2000/XP: Start Mail key
		// VK_LAUNCH_MEDIA_SELECT (B5) Windows 2000/XP: Select Media key
		// VK_LAUNCH_APP1 (B6) Windows 2000/XP: Start Application 1 key
		// VK_LAUNCH_APP2 (B7) Windows 2000/XP: Start Application 2 key

		// VK_OEM_1 (BA) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the ';:' key
	case Qt::Key_Semicolon:
	case Qt::Key_Colon:
		return VK_OEM_1; // case ';': case ':': return 0xBA;
		// VK_OEM_PLUS (BB) Windows 2000/XP: For any country/region, the '+' key
	case Qt::Key_Plus:
	case Qt::Key_Equal:
		return VK_OEM_PLUS; // case '=': case '+': return 0xBB;
		// VK_OEM_COMMA (BC) Windows 2000/XP: For any country/region, the ',' key
	case Qt::Key_Comma:
	case Qt::Key_Less:
		return VK_OEM_COMMA; // case ',': case '<': return 0xBC;
		// VK_OEM_MINUS (BD) Windows 2000/XP: For any country/region, the '-' key
	case Qt::Key_Minus:
	case Qt::Key_Underscore:
		return VK_OEM_MINUS; // case '-': case '_': return 0xBD;
		// VK_OEM_PERIOD (BE) Windows 2000/XP: For any country/region, the '.' key
	case Qt::Key_Period:
	case Qt::Key_Greater:
		return VK_OEM_PERIOD; // case '.': case '>': return 0xBE;
		// VK_OEM_2 (BF) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '/?' key
	case Qt::Key_Slash:
	case Qt::Key_Question:
		return VK_OEM_2; // case '/': case '?': return 0xBF;
		// VK_OEM_3 (C0) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '`~' key
	case Qt::Key_AsciiTilde:
	case Qt::Key_QuoteLeft:
		return VK_OEM_3; // case '`': case '~': return 0xC0;
		// VK_OEM_4 (DB) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '[{' key
	case Qt::Key_BracketLeft:
	case Qt::Key_BraceLeft:
		return VK_OEM_4; // case '[': case '{': return 0xDB;
		// VK_OEM_5 (DC) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '\|' key
	case Qt::Key_Backslash:
	case Qt::Key_Bar:
		return VK_OEM_5; // case '\\': case '|': return 0xDC;
		// VK_OEM_6 (DD) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the ']}' key
	case Qt::Key_BracketRight:
	case Qt::Key_BraceRight:
		return VK_OEM_6; // case ']': case '}': return 0xDD;
		// VK_OEM_7 (DE) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
	case Qt::Key_QuoteDbl:
		return VK_OEM_7; // case '\'': case '"': return 0xDE;
		// VK_OEM_8 (DF) Used for miscellaneous characters; it can vary by keyboard.
		// VK_OEM_102 (E2) Windows 2000/XP: Either the angle bracket key or the backslash key on the RT 102-key keyboard
		// VK_PROCESSKEY (E5) Windows 95/98/Me, Windows NT 4.0, Windows 2000/XP: IME PROCESS key
		// VK_PACKET (E7) Windows 2000/XP: Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT,SendInput, WM_KEYDOWN, and WM_KEYUP
		// VK_ATTN (F6) Attn key
		// VK_CRSEL (F7) CrSel key
		// VK_EXSEL (F8) ExSel key
		// VK_EREOF (F9) Erase EOF key
		// VK_PLAY (FA) Play key
		// VK_ZOOM (FB) Zoom key
		// VK_NONAME (FC) Reserved for future use
		// VK_PA1 (FD) PA1 key
		// VK_OEM_CLEAR (FE) Clear key
	default:
		return 0;
	}
}

int BFKeyMapper::mapQtKeyToX11KeyCode(const Qt::Key &qt_key, bool is_keypad_key)
{
	if (is_keypad_key)
	{
		switch (qt_key)
		{
		case Qt::Key_0:
			return XK_KP_0;
		case Qt::Key_1:
			return XK_KP_1;
		case Qt::Key_2:
			return XK_KP_2;
		case Qt::Key_3:
			return XK_KP_3;
		case Qt::Key_4:
			return XK_KP_4;
		case Qt::Key_5:
			return XK_KP_5;
		case Qt::Key_6:
			return XK_KP_6;
		case Qt::Key_7:
			return XK_KP_7;
		case Qt::Key_8:
			return XK_KP_8;
		case Qt::Key_9:
			return XK_KP_9;
		case Qt::Key_Asterisk:
			return XK_KP_Multiply;
		case Qt::Key_Plus:
			return XK_KP_Add;
		case Qt::Key_Minus:
			return XK_KP_Subtract;
		case Qt::Key_Period:
			return XK_KP_Decimal;
		case Qt::Key_Slash:
			return XK_KP_Divide;
		case Qt::Key_PageUp:
			return XK_Page_Up;
		case Qt::Key_PageDown:
			return XK_Page_Down;
		case Qt::Key_End:
			return XK_KP_End;
		case Qt::Key_Home:
			return XK_KP_Home;
		case Qt::Key_Left:
			return XK_KP_Left;
		case Qt::Key_Up:
			return XK_KP_Up;
		case Qt::Key_Right:
			return XK_KP_Right;
		case Qt::Key_Down:
			return XK_KP_Down;
		case Qt::Key_Enter:
		case Qt::Key_Return:
			return XK_KP_Enter;
		case Qt::Key_Insert:
			return XK_KP_Insert;
		case Qt::Key_Delete:
			return XK_KP_Delete;
		default:
			return 0;
		}
	}

	switch (qt_key)
	{
	case Qt::Key_Backspace:
		return XK_BackSpace;
	case Qt::Key_Backtab:
	case Qt::Key_Tab:
		return XK_Tab;
	case Qt::Key_Clear:
		return XK_Clear;
	case Qt::Key_Enter:
	case Qt::Key_Return:
		return XK_Return;
	case Qt::Key_Shift:
		return XK_Shift_L;
	case Qt::Key_Control:
		return XK_Control_L;
	case Qt::Key_Menu:
		return XK_Menu;
	case Qt::Key_Alt:
		return XK_Alt_L;

	case Qt::Key_F1:
		return XK_F1;
	case Qt::Key_F2:
		return XK_F2;
	case Qt::Key_F3:
		return XK_F3;
	case Qt::Key_F4:
		return XK_F4;
	case Qt::Key_F5:
		return XK_F5;
	case Qt::Key_F6:
		return XK_F6;
	case Qt::Key_F7:
		return XK_F7;
	case Qt::Key_F8:
		return XK_F8;
	case Qt::Key_F9:
		return XK_F9;
	case Qt::Key_F10:
		return XK_F10;
	case Qt::Key_F11:
		return XK_F11;
	case Qt::Key_F12:
		return XK_F12;
	case Qt::Key_F13:
		return XK_F13;
	case Qt::Key_F14:
		return XK_F14;
	case Qt::Key_F15:
		return XK_F15;
	case Qt::Key_F16:
		return XK_F16;
	case Qt::Key_F17:
		return XK_F17;
	case Qt::Key_F18:
		return XK_F18;
	case Qt::Key_F19:
		return XK_F19;
	case Qt::Key_F20:
		return XK_F20;
	case Qt::Key_F21:
		return XK_F21;
	case Qt::Key_F22:
		return XK_F22;
	case Qt::Key_F23:
		return XK_F23;
	case Qt::Key_F24:
		return XK_F24;

	case Qt::Key_Pause:
		return XK_Pause;
	case Qt::Key_CapsLock:
		return XK_Caps_Lock;
	case Qt::Key_Kana_Lock:
		return XK_Kana_Lock;
	case Qt::Key_Kana_Shift:
		return XK_Kana_Shift;
	case Qt::Key_Hangul:
		return XK_Hangul;
	case Qt::Key_Hangul_Hanja:
		return XK_Hangul_Hanja;
	case Qt::Key_Kanji:
		return XK_Kanji;
	case Qt::Key_Escape:
		return XK_Escape;
	case Qt::Key_Space:
		return XK_space;
	case Qt::Key_PageUp:
		return XK_Page_Up;
	case Qt::Key_PageDown:
		return XK_Page_Down;
	case Qt::Key_End:
		return XK_End;
	case Qt::Key_Home:
		return XK_Home;
	case Qt::Key_Left:
		return XK_Left;
	case Qt::Key_Up:
		return XK_Up;
	case Qt::Key_Right:
		return XK_Right;
	case Qt::Key_Down:
		return XK_Down;
	case Qt::Key_Select:
		return XK_Select;
	case Qt::Key_Print:
		return XK_Print;
	case Qt::Key_Execute:
		return XK_Execute;
	case Qt::Key_Insert:
		return XK_Insert;
	case Qt::Key_Delete:
		return XK_Delete;
	case Qt::Key_Help:
		return XK_Help;
	case Qt::Key_0:
		return XK_0;
	case Qt::Key_ParenLeft:
		return XK_parenleft;
	case Qt::Key_1:
		return XK_1;
	case Qt::Key_2:
	case Qt::Key_At:
		return XK_2;
	case Qt::Key_3:
		return Qt::Key_3;
	case Qt::Key_NumberSign:
		return XK_numbersign;
	case Qt::Key_4:
		return XK_4;
	case Qt::Key_Dollar:
		return XK_dollar;
	case Qt::Key_5:
		return XK_5;
	case Qt::Key_Percent:
		return XK_percent;
	case Qt::Key_6:
		return XK_6;
	case Qt::Key_AsciiCircum:
		return XK_asciicircum;
	case Qt::Key_7:
		return XK_7;
	case Qt::Key_Ampersand:
		return XK_ampersand;
	case Qt::Key_8:
		return XK_8;
	case Qt::Key_Asterisk:
		return XK_asterisk;
	case Qt::Key_9:
		return XK_9;
	case Qt::Key_ParenRight:
		return XK_parenright;
	case Qt::Key_A:
		return XK_A;
	case Qt::Key_B:
		return XK_B;
	case Qt::Key_C:
		return XK_C;
	case Qt::Key_D:
		return XK_D;
	case Qt::Key_E:
		return XK_E;
	case Qt::Key_F:
		return XK_F;
	case Qt::Key_G:
		return XK_G;
	case Qt::Key_H:
		return XK_H;
	case Qt::Key_I:
		return XK_I;
	case Qt::Key_J:
		return XK_J;
	case Qt::Key_K:
		return XK_K;
	case Qt::Key_L:
		return XK_L;
	case Qt::Key_M:
		return XK_M;
	case Qt::Key_N:
		return XK_N;
	case Qt::Key_O:
		return XK_O;
	case Qt::Key_P:
		return XK_P;
	case Qt::Key_Q:
		return XK_Q;
	case Qt::Key_R:
		return XK_R;
	case Qt::Key_S:
		return XK_S;
	case Qt::Key_T:
		return XK_T;
	case Qt::Key_U:
		return XK_U;
	case Qt::Key_V:
		return XK_V;
	case Qt::Key_W:
		return XK_W;
	case Qt::Key_X:
		return XK_X;
	case Qt::Key_Y:
		return XK_Y;
	case Qt::Key_Z:
		return XK_Z;

	case Qt::Key_Meta:
		return XK_Meta_L;
	case Qt::Key_NumLock:
		return XK_Num_Lock;
	case Qt::Key_ScrollLock:
		return XK_Scroll_Lock;
	case Qt::Key_Semicolon:
		return XK_semicolon;
	case Qt::Key_Colon:
		return XK_colon;
	case Qt::Key_Plus:
		return XK_plus;
	case Qt::Key_Equal:
		return XK_equal;
	case Qt::Key_Comma:
		return XK_comma;
	case Qt::Key_Less:
		return XK_less;
	case Qt::Key_Minus:
		return XK_minus;
	case Qt::Key_Underscore:
		return XK_underscore;
	case Qt::Key_Period:
		return XK_period;
	case Qt::Key_Greater:
		return XK_greater;
	case Qt::Key_Slash:
		return XK_slash;
	case Qt::Key_Question:
		return XK_question;
	case Qt::Key_AsciiTilde:
		return XK_asciitilde;
	case Qt::Key_QuoteLeft:
		return XK_quoteleft;
	case Qt::Key_BracketLeft:
		return XK_bracketleft;
	case Qt::Key_BraceLeft:
		return XK_braceleft;
	case Qt::Key_Backslash:
		return XK_backslash;
	case Qt::Key_Bar:
		return XK_bar;
	case Qt::Key_BracketRight:
		return XK_bracketright;
	case Qt::Key_BraceRight:
		return XK_braceright;
	case Qt::Key_QuoteDbl:
		return XK_quotedbl;
	default:
		return 0;
	}
}

int BFKeyMapper::mapQtKeyToNativeKeyCode(const Qt::Key &qt_key, bool is_keypad_key)
{
#ifdef Q_OS_LINUX
	return mapQtKeyToX11KeyCode(qt_key, is_keypad_key);
#else
	return mapQtKeyToWindowsKeyCode(qt_key, is_keypad_key);
#endif
}

int BFKeyMapper::mapQtKeyboardModifiersToCefKeyModifiers(const Qt::KeyboardModifiers &qt_keyboard_modifiers)
{
	int cef_modifiers = 0;
	if (qt_keyboard_modifiers & Qt::ShiftModifier)
	{
		cef_modifiers |= EVENTFLAG_SHIFT_DOWN;
	}
	if (qt_keyboard_modifiers & Qt::ShiftModifier)
	{
		cef_modifiers |= EVENTFLAG_CAPS_LOCK_ON;
	}
	if (qt_keyboard_modifiers & Qt::ControlModifier)
	{
		cef_modifiers |= EVENTFLAG_CONTROL_DOWN;
	}
	if (qt_keyboard_modifiers & Qt::AltModifier)
	{
		cef_modifiers |= EVENTFLAG_ALT_DOWN;
	}
	return cef_modifiers;
}
