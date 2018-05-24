#include "desktop.h"
#include <QThread>
#include <QPoint>
#include "x11_screen.h"
#include "uinput_faker.h"
#include "keymap.h"

class DesktopPrivate {
public:
	X11Screen *x11_screen;
	UinputFaker *uinput_faker;
	
	void sendMouseClick(int btn_code, int x, int y) {
		uinput_faker->warpCursor(x, y);
		QThread::msleep(120);
		uinput_faker->press(btn_code);
	}
};

Desktop::Desktop(QObject *parent) : QObject(parent), d_ptr(new DesktopPrivate)
{
	d_ptr->x11_screen = new X11Screen(nullptr);
	d_ptr->uinput_faker = new UinputFaker();
}

Desktop::~Desktop()
{
	
}

bool Desktop::takeScreenshot(cv::UMat &screenshot)
{
	return d_ptr->x11_screen->capture(screenshot);
}

int Desktop::getWidth()
{
	return d_ptr->x11_screen->getWidth();
}

int Desktop::getHeight()
{
	return d_ptr->x11_screen->getHeight();
}

void Desktop::leftClick(int x, int y)
{
	d_ptr->sendMouseClick(BTN_LEFT, x, y);
}

void Desktop::middleClick(int x, int y)
{
	d_ptr->sendMouseClick(BTN_MIDDLE, x, y);
}

void Desktop::rightClick(int x, int y)
{
	d_ptr->sendMouseClick(BTN_RIGHT, x, y);
}

void Desktop::pressKey(QString key) {
	d_ptr->uinput_faker->press(KEYMAP[key.toLower()]);
}

void Desktop::holdKey(QString key)
{
	d_ptr->uinput_faker->hold(KEYMAP[key.toLower()]);
}

void Desktop::releaseKey(QString key)
{
	d_ptr->uinput_faker->release(KEYMAP[key.toLower()]);
}

void Desktop::warpCursor(int x, int y)
{
	d_ptr->uinput_faker->warpCursor(x, y);
}

bool Desktop::getCursorPosition(int *x, int *y)
{
	return d_ptr->x11_screen->getCursorPosition(x, y);
}

bool Desktop::keyExists(QString key)
{
	return KEYMAP.contains(key.toLower());
}