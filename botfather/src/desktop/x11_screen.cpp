#include "x11_screen.h"

X11Screen::X11Screen(char* display_name)
{
	display = XOpenDisplay(display_name);
	root = DefaultRootWindow(display);
}

X11Screen::~X11Screen()
{
	XCloseDisplay(display);
}

bool X11Screen::capture(cv::UMat &output_umat)
{
	XWindowAttributes attributes;
	XGetWindowAttributes(display, root, &attributes);
	
	int width = attributes.width;
	int height = attributes.height;
	
	XImage *x_image = XGetImage(display, root, 0, 0, width, height, AllPlanes, ZPixmap);
	int bits_per_pixel = x_image->bits_per_pixel;
	
	cv::Mat mat(height, width, bits_per_pixel > 24 ? CV_8UC4 : CV_8UC3, x_image->data);
	
	if (bits_per_pixel > 24) {
		// Cut off the alpha channel (because there is a chance the image will be completely transparent otherwise).
		// And desktop screenshots usually don't feature any transparency at all.
		cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);
	}
	mat.copyTo(output_umat);
	
	XDestroyImage(x_image);
	return true;
}

QSize X11Screen::getSize()
{
	XWindowAttributes attributes;
	XGetWindowAttributes(display, root, &attributes);
	return QSize(attributes.width, attributes.height);
}

int X11Screen::getWidth()
{
	return getSize().width();
}

int X11Screen::getHeight()
{
	return getSize().height();
}