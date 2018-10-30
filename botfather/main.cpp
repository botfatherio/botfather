#include <QApplication>
#include <QSettings>
#include <QTime>
#include <QMessageBox>
#include "gui/control_window.h"
#include "engine/modules/browser/browser.h"

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialise the QApplication on Windows.
	QApplication a(__argc, __argv);
#else
#include <X11/Xlib.h>

namespace {

int handleXError(Display *display, XErrorEvent *event)
{
	char error_code_text_buffer[1024];
	XGetErrorText(display, event->error_code, error_code_text_buffer, sizeof(error_code_text_buffer));

	// TODO: Test whether using XGetErrorText with request codes is appropriate
	char request_code_text[1024];
	XGetErrorText(display, event->error_code, request_code_text, sizeof(request_code_text));

	// TODO: Test whether using XGetErrorText with minor codes is appropriate
	char minor_code_text[1024];
	XGetErrorText(display, event->minor_code, minor_code_text, sizeof(request_code_text));

	qDebug()
		<< "X error received. type: " << event->type
		<< ", serial: " << event->serial
		<< ", error: " << QString(error_code_text_buffer)
		<< ", request: " << QString(request_code_text)
		<< ", minor: " << QString(minor_code_text);
	return 0;
}

int handleXIOError(Display *)
{
	qDebug() << "QIOError handled.";
	return 0;
}

} // namespace

int main(int argc, char *argv[])
{
	// Intialise the QApplication on Linux and MacOS
	QApplication a(argc, argv);
	
	// Install xlib error handlers so that the application won't be terminated
	// on non-fatal errors.
	XSetErrorHandler(handleXError);
	XSetIOErrorHandler(handleXIOError);
#endif

	// Organization and application info must be set. Otherwise one can't access
	// QSettings from all over the app. Change this to match the bots credentials.
	QCoreApplication::setOrganizationName("botfather.io");
	QCoreApplication::setOrganizationDomain("botfather.io");
	QCoreApplication::setApplicationName("Botfather");
	QCoreApplication::setApplicationVersion("4.3.4");
	QSettings::setDefaultFormat(QSettings::IniFormat);

#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
    // qrand needs to be seeded once per thread
    qsrand(static_cast<uint>(QTime::currentTime().msec()));
#endif

	#if defined(_WIN32) || defined(_WIN64)
		Browser::init(__argc, __argv);
	#else
		Browser::init(argc, argv);
	#endif

	ControlWindow *control_window = new ControlWindow;
	control_window->show();

	// Runs the QApplication event loop blocking. When the event loop stops the timer
	// powering the CEF event loop will stop aswell. After that the CEF can be shut
	// down calling Browser::quit(). Subsequently both cef and qt event and message loops
	// stopped, the applications exit code can be returned.
	int return_code = a.exec();
	Browser::quit();
	return return_code;
}
