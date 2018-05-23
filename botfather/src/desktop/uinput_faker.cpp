#include "uinput_faker.h"
#include <unistd.h> // write, usleep
#include <string.h> // memset, strcpy
#include <fcntl.h> // open
#include <ctime> // time

UinputFaker::UinputFaker(char* display_name)
{
	// Uinput is a writable file which contains all mouse and keybord inputs.
	// They are preocessed by the kernel and then interpreted by the userspace.
	fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	
	// Enable key and relative events
	ioctl(fd, UI_SET_EVBIT, EV_KEY);
	ioctl(fd, UI_SET_EVBIT, EV_REL);
	
	// Enable key input events
	ioctl(fd, UI_SET_KEYBIT, KEY_1); // 1
	ioctl(fd, UI_SET_KEYBIT, KEY_2); // 2
	ioctl(fd, UI_SET_KEYBIT, KEY_3); // 3
	ioctl(fd, UI_SET_KEYBIT, KEY_4); // 4
	ioctl(fd, UI_SET_KEYBIT, KEY_5); // 5
	ioctl(fd, UI_SET_KEYBIT, KEY_6); // 6
	ioctl(fd, UI_SET_KEYBIT, KEY_7); // 7
	ioctl(fd, UI_SET_KEYBIT, KEY_8); // 8
	ioctl(fd, UI_SET_KEYBIT, KEY_9); // 9
	ioctl(fd, UI_SET_KEYBIT, KEY_0); // 0
	ioctl(fd, UI_SET_KEYBIT, KEY_A); // A
	ioctl(fd, UI_SET_KEYBIT, KEY_B); // B
	ioctl(fd, UI_SET_KEYBIT, KEY_C); // C
	ioctl(fd, UI_SET_KEYBIT, KEY_D); // D
	ioctl(fd, UI_SET_KEYBIT, KEY_E); // E
	ioctl(fd, UI_SET_KEYBIT, KEY_F); // F
	ioctl(fd, UI_SET_KEYBIT, KEY_G); // G
	ioctl(fd, UI_SET_KEYBIT, KEY_H); // H
	ioctl(fd, UI_SET_KEYBIT, KEY_I); // I
	ioctl(fd, UI_SET_KEYBIT, KEY_J); // J
	ioctl(fd, UI_SET_KEYBIT, KEY_K); // K
	ioctl(fd, UI_SET_KEYBIT, KEY_L); // L
	ioctl(fd, UI_SET_KEYBIT, KEY_M); // M
	ioctl(fd, UI_SET_KEYBIT, KEY_N); // N
	ioctl(fd, UI_SET_KEYBIT, KEY_O); // O
	ioctl(fd, UI_SET_KEYBIT, KEY_P); // P
	ioctl(fd, UI_SET_KEYBIT, KEY_Q); // Q
	ioctl(fd, UI_SET_KEYBIT, KEY_R); // R
	ioctl(fd, UI_SET_KEYBIT, KEY_S); // S
	ioctl(fd, UI_SET_KEYBIT, KEY_T); // T
	ioctl(fd, UI_SET_KEYBIT, KEY_U); // U
	ioctl(fd, UI_SET_KEYBIT, KEY_V); // V
	ioctl(fd, UI_SET_KEYBIT, KEY_W); // W
	ioctl(fd, UI_SET_KEYBIT, KEY_X); // X
	ioctl(fd, UI_SET_KEYBIT, KEY_Y); // Y
	ioctl(fd, UI_SET_KEYBIT, KEY_Z); // Z
	ioctl(fd, UI_SET_KEYBIT, KEY_LEFTALT); // LEFT ALT
	ioctl(fd, UI_SET_KEYBIT, KEY_LEFTCTRL); // LEFT CTRL
	ioctl(fd, UI_SET_KEYBIT, KEY_LEFTSHIFT); // LEFT SHIFT
	ioctl(fd, UI_SET_KEYBIT, KEY_RIGHTALT); // RIGHT ALT
	ioctl(fd, UI_SET_KEYBIT, KEY_RIGHTCTRL); // RIGHT CTRL
	ioctl(fd, UI_SET_KEYBIT, KEY_RIGHTSHIFT); // RIGHT SHIFT
	ioctl(fd, UI_SET_KEYBIT, KEY_SLASH); // SLASH
	ioctl(fd, UI_SET_KEYBIT, KEY_BACKSLASH); // BACKSLASH
	ioctl(fd, UI_SET_KEYBIT, KEY_COMMA); // COMMA
	ioctl(fd, UI_SET_KEYBIT, KEY_DOT); // DOT
	ioctl(fd, UI_SET_KEYBIT, KEY_SPACE); // SPACE
	ioctl(fd, UI_SET_KEYBIT, KEY_BACKSPACE); // {BACKSPACE}, {BS}, or {BKSP} 
	ioctl(fd, UI_SET_KEYBIT, KEY_BREAK); // {BREAK} 
	ioctl(fd, UI_SET_KEYBIT, KEY_CAPSLOCK); // {CAPSLOCK}
	ioctl(fd, UI_SET_KEYBIT, KEY_DELETE); // {DELETE} or {DEL} 
	ioctl(fd, UI_SET_KEYBIT, KEY_DOWN); // {DOWN}
	ioctl(fd, UI_SET_KEYBIT, KEY_END); // {END}
	ioctl(fd, UI_SET_KEYBIT, KEY_ENTER); // ENTER {ENTER}or ~
	ioctl(fd, UI_SET_KEYBIT, KEY_ESC); // ESC {ESC}
	ioctl(fd, UI_SET_KEYBIT, KEY_LEFT); // LEFT ARROW {LEFT}
	ioctl(fd, UI_SET_KEYBIT, KEY_PAGEDOWN); // PAGE DOWN {PGDN}
	ioctl(fd, UI_SET_KEYBIT, KEY_PAGEUP); // PAGE UP {PGUP}
	ioctl(fd, UI_SET_KEYBIT, KEY_RIGHT); // RIGHT ARROW {RIGHT}
	ioctl(fd, UI_SET_KEYBIT, KEY_TAB); // TAB {TAB}
	ioctl(fd, UI_SET_KEYBIT, KEY_UP); // UP ARROW {UP}
	ioctl(fd, UI_SET_KEYBIT, KEY_F1); // F1 {F1}
	ioctl(fd, UI_SET_KEYBIT, KEY_F2); // F2 {F2}
	ioctl(fd, UI_SET_KEYBIT, KEY_F3); // F2 F3 {F3}
	ioctl(fd, UI_SET_KEYBIT, KEY_F4); // F2 F4 {F4}	
	ioctl(fd, UI_SET_KEYBIT, KEY_F5); // F2 F5 {F5}	
	ioctl(fd, UI_SET_KEYBIT, KEY_F6); // F2 F6 {F6}	
	ioctl(fd, UI_SET_KEYBIT, KEY_F7); // F2 F7 {F7}	
	ioctl(fd, UI_SET_KEYBIT, KEY_F8); // F2 F8 {F8}	
	ioctl(fd, UI_SET_KEYBIT, KEY_F9); // F2 F9 {F9}	
	ioctl(fd, UI_SET_KEYBIT, KEY_F10); // F2 F10 {F10}	
	ioctl(fd, UI_SET_KEYBIT, KEY_F11); // F2 F11 {F11}	
	ioctl(fd, UI_SET_KEYBIT, KEY_F12); // F2 F12 {F12}	
	ioctl(fd, UI_SET_KEYBIT, KEY_F13); // F2 F13 {F13}	
	ioctl(fd, UI_SET_KEYBIT, KEY_F14); // F2 F14 {F14}	
	ioctl(fd, UI_SET_KEYBIT, KEY_F15); // F2 F15 {F15}	
	ioctl(fd, UI_SET_KEYBIT, KEY_F16); // F2 F16 {F16}
	
	// Enable mouse movement input
	ioctl(fd, UI_SET_RELBIT, REL_X);
	ioctl(fd, UI_SET_RELBIT, REL_Y);
	
	// Mouse button events (they don't require mouse movement to be enabled)
	ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
	ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);
	ioctl(fd, UI_SET_KEYBIT, BTN_MIDDLE);
	ioctl(fd, UI_SET_KEYBIT, BTN_WHEEL);
	ioctl(fd, UI_SET_KEYBIT, BTN_GEAR_DOWN);
	ioctl(fd, UI_SET_KEYBIT, BTN_GEAR_UP);
	
	// Create a fake input device, which is capable of inputting/faking
	// mouse and keyboard inputs.
	memset(&usetup, 0, sizeof(usetup));
	usetup.id.bustype = BUS_USB;
	usetup.id.vendor = 0x1234; // Sample vendor
	usetup.id.product = 0x5678; // Sample product
	strcpy(usetup.name, "BFP Universal Input Device"); // Device name
	
	// Enable the fake input device.
	ioctl(fd, UI_DEV_SETUP, &usetup);
	ioctl(fd, UI_DEV_CREATE);
	
	display = XOpenDisplay(display_name);
	root_window = XDefaultRootWindow(display);
}

UinputFaker::~UinputFaker()
{
	ioctl(fd, UI_DEV_DESTROY);
	close(fd);
	XCloseDisplay(display);
}

void UinputFaker::hold(int key_or_btn_code)
{
	emitEvent(EV_KEY, key_or_btn_code, 1);
	emitEvent(EV_SYN, SYN_REPORT, 0);
}

void UinputFaker::release(int key_or_btn_code)
{
	emitEvent(EV_KEY, key_or_btn_code, 0);
	emitEvent(EV_SYN, SYN_REPORT, 0);
}

void UinputFaker::press(int key_or_btn_code)
{
	hold(key_or_btn_code);
	release(key_or_btn_code);
}

void UinputFaker::moveCursor(int delta_x, int delta_y)
{
	emitEvent(EV_REL, REL_X, delta_x);
	emitEvent(EV_REL, REL_Y, delta_y);
	emitEvent(EV_SYN, SYN_REPORT, 0);
}

void UinputFaker::warpCursor(int x, int y)
{
	// Using uinput the cursor can only be moved relative to it's current possition as hardware mouse
	// input is naturally relative. (One can move it's mouse as far as he wants in one direction
	// without hitting any physical barrier).
	// The following trick doesn't work: We cant move the mouse relatively -9999 -9999 to get the mouse
	// to the top left corner of the screen. But moving it then by 100 100 doesn't result in the mouse
	// being at 100 100 unfortunately. For some reason it works when the mouse has been placed at 0 0
	// before manually.
	// The Xlib on the otherhand allows us to warp the pointer/cursor to a absolute position. But
	// mouseinput generated by Xlib is flagged as fake input and thus ignored by some programs,
	// particulary games.
	// We use a combined efford to move the mouse to the desired position while hacing it accepted by all
	// programs: We move the mouse using xlib to the desired position and then shake the cursor a little
	// using input to make all programs recognise the cursors new position.
	
	// If dest_w is None, XWarpPointer moves the pointer by the offsets (dest_x, dest_y)
	// relative to the current position of the pointer. https://linux.die.net/man/3/xwarppointer
	XWarpPointer(display, root_window, root_window, 0, 0, 0, 0, x, y);
	XFlush(display);
	
	// Shake the mouse using uinput
	moveCursor(10, 10);
	moveCursor(-10, -10);
}

void UinputFaker::emitEvent(int type, int code, int val)
{
	struct input_event ie;
	
	ie.type = type;
	ie.code = code;
	ie.value = val;
	
	// Timestamp values below are ignored
	ie.time.tv_sec = std::time(0);
	ie.time.tv_usec = 0;
	
	write(fd, &ie, sizeof(ie));
}