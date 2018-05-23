#ifndef BFP__DESKTOP__WINPUT_KEYMAP_H
#define BFP__DESKTOP__WINPUT_KEYMAP_H

#include <QHash>
#include <WinUser.h>

// All virtual key codesc
// https://msdn.microsoft.com/de-de/library/windows/desktop/dd375731(v=vs.85).aspx

static const QHash<QString, WORD> WINPUT_KEYMAP = {
    // Normal numbers
    {"0", 0x30},
    {"1", 0x31},
    {"2", 0x32},
    {"3", 0x33},
    {"4", 0x34},
    {"5", 0x35},
    {"6", 0x36},
    {"7", 0x37},
    {"8", 0x38},
    {"9", 0x39},

    // Small letters
    {"a", 0x41},
    {"b", 0x42},
    {"c", 0x43},
    {"d", 0x44},
    {"e", 0x45},
    {"f", 0x46},
    {"g", 0x47},
    {"h", 0x48},
    {"i", 0x49},
    {"j", 0x4A},
    {"k", 0x4B},
    {"l", 0x4C},
    {"m", 0x4D},
    {"n", 0x4E},
    {"o", 0x4F},
    {"p", 0x50},
    {"q", 0x51},
    {"r", 0x52},
    {"s", 0x53},
    {"t", 0x54},
    {"u", 0x55},
    {"v", 0x56},
    {"w", 0x57},
    {"x", 0x58},
    {"y", 0x59},
    {"z", 0x5A},

    // F Keys
    {"f1", VK_F1},
    {"f2", VK_F2},
    {"f3", VK_F3},
    {"f4", VK_F4},
    {"f5", VK_F5},
    {"f6", VK_F6},
    {"f7", VK_F7},
    {"f8", VK_F8},
    {"f9", VK_F9},
    {"f10", VK_F10},
    {"f11", VK_F11},
    {"f12", VK_F12},
    {"f13", VK_F13},
    {"f14", VK_F14},
    {"f15", VK_F15},
    {"f16", VK_F16},
    {"f17", VK_F17},
    {"f18", VK_F18},
    {"f19", VK_F19},
    {"f20", VK_F20},
    {"f21", VK_F21},
    {"f22", VK_F22},
    {"f23", VK_F23},
    {"f24", VK_F24},

    // Arrow keys
    {"left", VK_LEFT},
    {"right", VK_RIGHT},
    {"up", VK_UP},
    {"down", VK_DOWN},

    // Special keys
    {"alt", VK_MENU},
    {"lmenu", VK_LMENU},
    {"rmenu", VK_RMENU},

    {"shift", VK_SHIFT},
    {"lshift", VK_LSHIFT},
    {"rshift", VK_RSHIFT},

    {"control", VK_CONTROL},
    {"lcontrol", VK_LCONTROL},
    {"rcontrol", VK_RCONTROL},

    {"capital", VK_CAPITAL},
    {"caps", VK_CAPITAL},
    {"capslock", VK_CAPITAL},
    {"caps_lock", VK_CAPITAL},

    {"space", VK_SPACE},
    {"tab", VK_TAB},
    {"esc", VK_ESCAPE},
    {"escape", VK_ESCAPE},

    {"back", VK_BACK},
    {"backspace", VK_BACK},
    {"del", VK_DELETE},
    {"delete", VK_DELETE},
    {"enter", VK_RETURN},
    {"return", VK_RETURN},

    // Volume keys
    {"volup", VK_VOLUME_UP},
    {"volumeup", VK_VOLUME_UP},
    {"volume_up", VK_VOLUME_UP},

    {"voldown", VK_VOLUME_DOWN},
    {"volumedown", VK_VOLUME_DOWN},
    {"volume_down", VK_VOLUME_DOWN}
};

#endif // BFP__DESKTOP__WINPUT_KEYMAP_H
