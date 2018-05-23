#ifndef BFP__DESKTOP__X11_SCREEN_H
#define BFP__DESKTOP__X11_SCREEN_H

#include <opencv2/opencv.hpp>
#include <QSize>
#include <X11/Xlib.h> // Import X11 stuff always last. Otherwise compilation fails.
#include <X11/Xutil.h>

class X11Screen
{
public:
	X11Screen(char* display_name=nullptr);
	~X11Screen();
	
	bool capture(cv::UMat &output_umat);
	QSize getSize();
	int getWidth();
	int getHeight();

private:
	Display *display;
	Window root;
};

#endif // BFP__DESKTOP__X11_SCREEN_H