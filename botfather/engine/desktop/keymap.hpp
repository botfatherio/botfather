#ifndef BFP_ENGINE_MODULES_DESKTOP_KEYMAP_H
#define BFP_ENGINE_MODULES_DESKTOP_KEYMAP_H

#include <QHash>

#ifdef Q_OS_LINUX
#include <X11/Xlib.h>

#include "linux/input-event-codes.hpp"
#define KEY_TYPE KeySym
#define KEY_CODE(windows_key, uinput_key) (uinput_key)
#elif defined(Q_OS_WIN)
#include <WinUser.h>
#define KEY_TYPE WORD
#define KEY_CODE(windows_key, uinput_key) (windows_key)
#endif

// Virtual key codes (windows)
// https://msdn.microsoft.com/de-de/library/windows/desktop/dd375731(v=vs.85).aspx

// Event codes (linux)
// https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h

static const QHash<QString, KEY_TYPE> KEYMAP = {

    // Normal numbers
    {"0", KEY_CODE(0x30, KEY_0)},
    {"1", KEY_CODE(0x31, KEY_1)},
    {"2", KEY_CODE(0x32, KEY_2)},
    {"3", KEY_CODE(0x33, KEY_3)},
    {"4", KEY_CODE(0x34, KEY_4)},
    {"5", KEY_CODE(0x35, KEY_5)},
    {"6", KEY_CODE(0x36, KEY_6)},
    {"7", KEY_CODE(0x37, KEY_7)},
    {"8", KEY_CODE(0x38, KEY_8)},
    {"9", KEY_CODE(0x39, KEY_9)},

    // Small letters
    {"a", KEY_CODE(0x41, KEY_A)},
    {"b", KEY_CODE(0x42, KEY_B)},
    {"c", KEY_CODE(0x43, KEY_C)},
    {"d", KEY_CODE(0x44, KEY_D)},
    {"e", KEY_CODE(0x45, KEY_E)},
    {"f", KEY_CODE(0x46, KEY_F)},
    {"g", KEY_CODE(0x47, KEY_G)},
    {"h", KEY_CODE(0x48, KEY_H)},
    {"i", KEY_CODE(0x49, KEY_I)},
    {"j", KEY_CODE(0x4A, KEY_J)},
    {"k", KEY_CODE(0x4B, KEY_K)},
    {"l", KEY_CODE(0x4C, KEY_L)},
    {"m", KEY_CODE(0x4D, KEY_M)},
    {"n", KEY_CODE(0x4E, KEY_N)},
    {"o", KEY_CODE(0x4F, KEY_O)},
    {"p", KEY_CODE(0x50, KEY_P)},
    {"q", KEY_CODE(0x51, KEY_Q)},
    {"r", KEY_CODE(0x52, KEY_R)},
    {"s", KEY_CODE(0x53, KEY_S)},
    {"t", KEY_CODE(0x54, KEY_T)},
    {"u", KEY_CODE(0x55, KEY_U)},
    {"v", KEY_CODE(0x56, KEY_V)},
    {"w", KEY_CODE(0x57, KEY_W)},
    {"x", KEY_CODE(0x58, KEY_X)},
    {"y", KEY_CODE(0x59, KEY_Y)},
    {"z", KEY_CODE(0x5A, KEY_Z)},

    // F Keys
    {"f1", KEY_CODE(VK_F1, KEY_F1)},
    {"f2", KEY_CODE(VK_F2, KEY_F2)},
    {"f3", KEY_CODE(VK_F3, KEY_F3)},
    {"f4", KEY_CODE(VK_F4, KEY_F4)},
    {"f5", KEY_CODE(VK_F5, KEY_F5)},
    {"f6", KEY_CODE(VK_F6, KEY_F6)},
    {"f7", KEY_CODE(VK_F7, KEY_F7)},
    {"f8", KEY_CODE(VK_F8, KEY_F8)},
    {"f9", KEY_CODE(VK_F9, KEY_F9)},
    {"f10", KEY_CODE(VK_F10, KEY_F10)},
    {"f11", KEY_CODE(VK_F11, KEY_F11)},
    {"f12", KEY_CODE(VK_F12, KEY_F12)},
    {"f13", KEY_CODE(VK_F13, KEY_F13)},
    {"f14", KEY_CODE(VK_F14, KEY_F14)},
    {"f15", KEY_CODE(VK_F15, KEY_F15)},
    {"f16", KEY_CODE(VK_F16, KEY_F16)},
    {"f17", KEY_CODE(VK_F17, KEY_F17)},
    {"f18", KEY_CODE(VK_F18, KEY_F18)},
    {"f19", KEY_CODE(VK_F19, KEY_F19)},
    {"f20", KEY_CODE(VK_F20, KEY_F20)},
    {"f21", KEY_CODE(VK_F21, KEY_F21)},
    {"f22", KEY_CODE(VK_F22, KEY_F22)},
    {"f23", KEY_CODE(VK_F23, KEY_F23)},
    {"f24", KEY_CODE(VK_F24, KEY_F24)},

    // Arrow keys
    {"left", KEY_CODE(VK_LEFT, KEY_LEFT)},
    {"right", KEY_CODE(VK_RIGHT, KEY_RIGHT)},
    {"up", KEY_CODE(VK_UP, KEY_UP)},
    {"down", KEY_CODE(VK_DOWN, KEY_DOWN)},

    // Special keys
    {"alt", KEY_CODE(VK_MENU, KEY_LEFTALT)},

    {"shift", KEY_CODE(VK_SHIFT, KEY_LEFTSHIFT)},
    {"lshift", KEY_CODE(VK_LSHIFT, KEY_LEFTSHIFT)},
    {"rshift", KEY_CODE(VK_RSHIFT, KEY_RIGHTSHIFT)},

    {"control", KEY_CODE(VK_CONTROL, KEY_LEFTCTRL)},
    {"lcontrol", KEY_CODE(VK_LCONTROL, KEY_LEFTCTRL)},
    {"rcontrol", KEY_CODE(VK_RCONTROL, KEY_RIGHTCTRL)},

    {"capital", KEY_CODE(VK_CAPITAL, KEY_CAPSLOCK)},
    {"caps", KEY_CODE(VK_CAPITAL, KEY_CAPSLOCK)},
    {"capslock", KEY_CODE(VK_CAPITAL, KEY_CAPSLOCK)},
    {"caps_lock", KEY_CODE(VK_CAPITAL, KEY_CAPSLOCK)},

    {"space", KEY_CODE(VK_SPACE, KEY_SPACE)},
    {"tab", KEY_CODE(VK_TAB, KEY_TAB)},
    {"esc", KEY_CODE(VK_ESCAPE, KEY_ESC)},
    {"escape", KEY_CODE(VK_ESCAPE, KEY_ESC)},

    {"backspace", KEY_CODE(VK_BACK, KEY_BACKSPACE)},
    {"del", KEY_CODE(VK_DELETE, KEY_DELETE)},
    {"delete", KEY_CODE(VK_DELETE, KEY_DELETE)},
    {"enter", KEY_CODE(VK_RETURN, KEY_ENTER)},
    {"return", KEY_CODE(VK_RETURN, KEY_ENTER)},

    // Volume keys
    {"volup", KEY_CODE(VK_VOLUME_UP, KEY_VOLUMEUP)},
    {"volumeup", KEY_CODE(VK_VOLUME_UP, KEY_VOLUMEUP)},
    {"volume_up", KEY_CODE(VK_VOLUME_UP, KEY_VOLUMEUP)},

    {"voldown", KEY_CODE(VK_VOLUME_DOWN, KEY_VOLUMEDOWN)},
    {"volumedown", KEY_CODE(VK_VOLUME_DOWN, KEY_VOLUMEDOWN)},
    {"volume_down", KEY_CODE(VK_VOLUME_DOWN, KEY_VOLUMEDOWN)}};

#endif  // BFP_ENGINE_MODULES_DESKTOP_KEYMAP_H
