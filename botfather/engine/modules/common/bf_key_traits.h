#ifndef BF_KEY_TRAITS_H
#define BF_KEY_TRAITS_H

#include <Qt>
#include <QChar>

struct BFKeyTraits
{
	Qt::Key qt_key = Qt::Key_unknown;
	QChar q_char = QChar();
	Qt::KeyboardModifier qt_keyboard_modifier = Qt::NoModifier;

	BFKeyTraits() {}

	explicit BFKeyTraits(const Qt::Key &qt_key)
		: qt_key(qt_key)
	{}

	BFKeyTraits(const Qt::Key &qt_key, const QChar &q_char)
		: qt_key(qt_key), q_char(q_char)
	{}

	BFKeyTraits(const Qt::Key &qt_key, const QChar &q_char, const Qt::KeyboardModifier &qt_keyboard_modifier)
		: qt_key(qt_key), q_char(q_char), qt_keyboard_modifier(qt_keyboard_modifier)
	{}
};

#endif // BF_KEY_TRAITS_H
