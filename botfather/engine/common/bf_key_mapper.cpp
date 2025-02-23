#include "bf_key_mapper.hpp"

#include <include/cef_base.h>

#include <QDebug>
#include <QHash>

#include "bf_keymap.hpp"
#include "linux11_keyboard_codes.hpp"
#include "qt_keymaps.hpp"
#include "windows_keyboard_codes.hpp"

Qt::KeyboardModifier BFKeyMapper::mapBFKeycodeToQtKeyboardModifier(
    const QString &bf_keycode) {
    if (BF_KEYMAP.contains(bf_keycode)) {
        return BF_KEYMAP[bf_keycode].qt_keyboard_modifier;
    }
    return Qt::NoModifier;
}

Qt::Key BFKeyMapper::mapBFKeycodeToQtKey(const QString &bf_keycode) {
    if (BF_KEYMAP.contains(bf_keycode)) {
        return BF_KEYMAP[bf_keycode].qt_key;
    }
    return Qt::Key();
}

QChar BFKeyMapper::mapBFKeycodeToQChar(const QString &bf_keycode) {
    if (BF_KEYMAP.contains(bf_keycode)) {
        return BF_KEYMAP[bf_keycode].q_char;
    }
    return QChar();
}

int BFKeyMapper::mapQtKeyToWindowsKeyCode(const Qt::Key &qt_key,
                                          bool is_keypad_key) {
    const QHash<const Qt::Key, QtKeyTraits> *keymap =
        is_keypad_key ? &QT_NUMPAD_KEYMAP : &QT_NORMAL_KEYMAP;
    if (keymap->contains(qt_key)) {
        return keymap->value(qt_key).windows_key_code;
    }
    return VK_UNKNOWN;
}

int BFKeyMapper::mapQtKeyToX11KeyCode(const Qt::Key &qt_key,
                                      bool is_keypad_key) {
    const QHash<const Qt::Key, QtKeyTraits> *keymap =
        is_keypad_key ? &QT_NUMPAD_KEYMAP : &QT_NORMAL_KEYMAP;
    if (keymap->contains(qt_key)) {
        return keymap->value(qt_key).linux11_key_code;
    }
    return XK_VoidSymbol;
}

int BFKeyMapper::mapQtKeyToNativeKeyCode(const Qt::Key &qt_key,
                                         bool is_keypad_key) {
#ifdef Q_OS_LINUX
    return mapQtKeyToX11KeyCode(qt_key, is_keypad_key);
#else
    return mapQtKeyToWindowsKeyCode(qt_key, is_keypad_key);
#endif
}

int BFKeyMapper::mapQtKeyboardModifiersToCefKeyModifiers(
    const Qt::KeyboardModifiers &qt_keyboard_modifiers) {
    int cef_modifiers = 0;
    if (qt_keyboard_modifiers & Qt::ShiftModifier) {
        cef_modifiers |= EVENTFLAG_SHIFT_DOWN;
    }
    if (qt_keyboard_modifiers & Qt::ShiftModifier) {
        cef_modifiers |= EVENTFLAG_CAPS_LOCK_ON;
    }
    if (qt_keyboard_modifiers & Qt::ControlModifier) {
        cef_modifiers |= EVENTFLAG_CONTROL_DOWN;
    }
    if (qt_keyboard_modifiers & Qt::AltModifier) {
        cef_modifiers |= EVENTFLAG_ALT_DOWN;
    }
    return cef_modifiers;
}
