#ifndef BF_KEYMAP_H
#define BF_KEYMAP_H

#include <QHash>
#include "bf_key_traits.h"

static const QHash<const QString, BFKeyTraits> BF_KEYMAP = {
	// Lowercase Letters
	{"a", {Qt::Key_A, QChar('a')}},
	{"b", {Qt::Key_B, QChar('b')}},
	{"c", {Qt::Key_C, QChar('c')}},
	{"d", {Qt::Key_D, QChar('d')}},
	{"e", {Qt::Key_E, QChar('e')}},
	{"f", {Qt::Key_F, QChar('f')}},
	{"g", {Qt::Key_G, QChar('g')}},
	{"h", {Qt::Key_H, QChar('h')}},
	{"i", {Qt::Key_I, QChar('i')}},
	{"j", {Qt::Key_J, QChar('j')}},
	{"k", {Qt::Key_K, QChar('k')}},
	{"l", {Qt::Key_L, QChar('l')}},
	{"m", {Qt::Key_M, QChar('m')}},
	{"n", {Qt::Key_N, QChar('n')}},
	{"o", {Qt::Key_O, QChar('o')}},
	{"p", {Qt::Key_P, QChar('p')}},
	{"q", {Qt::Key_Q, QChar('q')}},
	{"r", {Qt::Key_R, QChar('r')}},
	{"s", {Qt::Key_S, QChar('s')}},
	{"t", {Qt::Key_T, QChar('t')}},
	{"u", {Qt::Key_U, QChar('u')}},
	{"v", {Qt::Key_V, QChar('v')}},
	{"w", {Qt::Key_W, QChar('w')}},
	{"x", {Qt::Key_X, QChar('x')}},
	{"y", {Qt::Key_Y, QChar('y')}},
	{"z", {Qt::Key_Z, QChar('z')}},

	// Uppercase Letters
	{"A", {Qt::Key_A, QChar('A')}},
	{"B", {Qt::Key_B, QChar('B')}},
	{"C", {Qt::Key_C, QChar('C')}},
	{"D", {Qt::Key_D, QChar('D')}},
	{"E", {Qt::Key_E, QChar('E')}},
	{"F", {Qt::Key_F, QChar('F')}},
	{"G", {Qt::Key_G, QChar('G')}},
	{"H", {Qt::Key_H, QChar('H')}},
	{"I", {Qt::Key_I, QChar('I')}},
	{"J", {Qt::Key_J, QChar('J')}},
	{"K", {Qt::Key_K, QChar('K')}},
	{"L", {Qt::Key_L, QChar('L')}},
	{"M", {Qt::Key_M, QChar('M')}},
	{"N", {Qt::Key_N, QChar('N')}},
	{"O", {Qt::Key_O, QChar('O')}},
	{"P", {Qt::Key_P, QChar('P')}},
	{"Q", {Qt::Key_Q, QChar('Q')}},
	{"R", {Qt::Key_R, QChar('R')}},
	{"S", {Qt::Key_S, QChar('S')}},
	{"T", {Qt::Key_T, QChar('T')}},
	{"U", {Qt::Key_U, QChar('U')}},
	{"V", {Qt::Key_V, QChar('V')}},
	{"W", {Qt::Key_W, QChar('W')}},
	{"X", {Qt::Key_X, QChar('X')}},
	{"Y", {Qt::Key_Y, QChar('Y')}},
	{"Z", {Qt::Key_Z, QChar('Z')}},

	// Numbers
	{"0", {Qt::Key_0, QChar('0')}},
	{"1", {Qt::Key_1, QChar('1')}},
	{"2", {Qt::Key_2, QChar('2')}},
	{"3", {Qt::Key_3, QChar('3')}},
	{"4", {Qt::Key_4, QChar('4')}},
	{"5", {Qt::Key_5, QChar('5')}},
	{"6", {Qt::Key_6, QChar('6')}},
	{"7", {Qt::Key_7, QChar('7')}},
	{"8", {Qt::Key_8, QChar('8')}},
	{"9", {Qt::Key_9, QChar('9')}},

	// Modifiers
	{"CTRL", BFKeyTraits(Qt::Key_Control, QChar(), Qt::ControlModifier)},
	{"ALT", BFKeyTraits(Qt::Key_Alt, QChar(), Qt::AltModifier)},
	{"META", BFKeyTraits(Qt::Key_Meta, QChar(), Qt::MetaModifier)},
	{"SHIFT", BFKeyTraits(Qt::Key_Shift, QChar(), Qt::ShiftModifier)},

	// Directions
	{"LEFT", BFKeyTraits(Qt::Key_Left, QChar())},
	{"RIGHT", BFKeyTraits(Qt::Key_Right, QChar())},
	{"UP", BFKeyTraits(Qt::Key_Up, QChar())},
	{"DOWN", BFKeyTraits(Qt::Key_Down, QChar())},

	// Misc
	{"SPACE", {Qt::Key_Space, QChar(' ')}},
	{"TAB", {Qt::Key_Tab, QChar('\t')}},
	{"ESC", {Qt::Key_Escape, QChar('\u001B')}},
	{"ENTER", {Qt::Key_Enter, QChar('\r')}},
	{"BACKSPACE", {Qt::Key_Backspace, QChar('\b')}},
	{"DEL", {Qt::Key_Delete, QChar('\u007F')}}
};

#endif // BF_KEYMAP_H
