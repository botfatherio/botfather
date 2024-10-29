#include "cef_key_event_adapter.hpp"

#include <QDebug>

#include "../common/bf_key_mapper.hpp"

CefKeyEventAdapter::CefKeyEventAdapter(const QKeyEvent *event) {
    bool is_keypad_key = event->modifiers() & Qt::KeypadModifier;

    modifiers = static_cast<uint32>(
        BFKeyMapper::mapQtKeyboardModifiersToCefKeyModifiers(
            event->modifiers()));
    native_key_code = static_cast<int>(event->nativeScanCode());
    windows_key_code = BFKeyMapper::mapQtKeyToWindowsKeyCode(
        Qt::Key(event->key()), is_keypad_key);

    // Even though the following code works perfectly under windows, I decided
    // to use the same code used under linux for consitency. windows_key_code =
    // static_cast<int>(event->nativeVirtualKey());

    if (!event->text().isEmpty()) {
        character = event->text().at(0).unicode();
        unmodified_character =
            event->text().at(0).unicode();  // Not quite correct I guess
    }

    // This value will always be false on non-Windows platforms.
    // Under windows Alt+AnyKey and F10 are considered system keys. But there is
    // no need for us to handle them different than other key events.
    is_system_key = false;
}

CefKeyEventAdapter::CefKeyEventAdapter(
    const QString &bf_keycode,
    const Qt::KeyboardModifiers &qt_keyboard_modifiers) {
    Qt::Key qt_key = BFKeyMapper::mapBFKeycodeToQtKey(bf_keycode);
    bool is_keypad_key = qt_keyboard_modifiers & Qt::KeypadModifier;

    modifiers = static_cast<uint32>(
        BFKeyMapper::mapQtKeyboardModifiersToCefKeyModifiers(
            qt_keyboard_modifiers));
    native_key_code =
        BFKeyMapper::mapQtKeyToNativeKeyCode(qt_key, is_keypad_key);
    windows_key_code =
        BFKeyMapper::mapQtKeyToWindowsKeyCode(qt_key, is_keypad_key);

    QChar corresponding_char = BFKeyMapper::mapBFKeycodeToQChar(bf_keycode);
    if (!corresponding_char.isNull()) {
        character = corresponding_char.unicode();
        unmodified_character = corresponding_char.unicode();
    } else if (bf_keycode.length() == 1) {
        // These characters are not mapped and might get interpreted wrong.
        // Most times interpreting them works. We could even stop mapping
        // Qt::Keys to QChars completely.
        character = bf_keycode.at(0).unicode();
        unmodified_character = bf_keycode.at(0).unicode();
    }
}
