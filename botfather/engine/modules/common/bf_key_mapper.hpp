#ifndef BF_KEY_MAPPER_H
#define BF_KEY_MAPPER_H

#include <QString>

namespace BFKeyMapper {
Qt::KeyboardModifier mapBFKeycodeToQtKeyboardModifier(
    const QString &bf_keycode);
Qt::Key mapBFKeycodeToQtKey(const QString &bf_keycode);
QChar mapBFKeycodeToQChar(const QString &bf_keycode);
int mapQtKeyToWindowsKeyCode(const Qt::Key &qt_key, bool is_keypad_key);
int mapQtKeyToX11KeyCode(const Qt::Key &qt_key, bool is_keypad_key);
int mapQtKeyToNativeKeyCode(const Qt::Key &qt_key, bool is_keypad_key);
int mapQtKeyboardModifiersToCefKeyModifiers(
    const Qt::KeyboardModifiers &qt_keyboard_modifiers);
}  // namespace BFKeyMapper

#endif  // BF_KEY_MAPPER_H
