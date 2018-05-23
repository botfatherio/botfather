#include "desktop.h"
#include <windows.h>

class DesktopPrivate {
public:

};

Desktop::Desktop(QObject *parent) : QObject(parent), d_ptr(new DesktopPrivate)
{

}

Desktop::~Desktop()
{
	
}

bool Desktop::takeScreenshot(cv::UMat &screenshot)
{
	
}

int Desktop::getWidth()
{
	
}

int Desktop::getHeight()
{
	
}

void Desktop::leftClick(int x, int y)
{
	
}

void Desktop::middleClick(int x, int y)
{
	
}

void Desktop::rightClick(int x, int y)
{
	
}

void Desktop::pressKey(QString key) {
	
}

void Desktop::holdKey(QString key)
{
	
}

void Desktop::releaseKey(QString key)
{
	
}

void Desktop::warpCursor(int x, int y)
{
	
}

bool Desktop::keyExists(QString key)
{
	
}