#include <QApplication>
#include <QSettings>
#include <QTime>
#include "gui/update_widget.h"
#include "gui/auth_window.h"
#include "gui/control_window.h"
#include "browser/browser.h"

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialise the QApplication on Windows.
	QApplication a(__argc, __argv);
#else

int main(int argc, char *argv[])
{
	// Intialise the QApplication on Linux and MacOS
	QApplication a(argc, argv);
#endif

	// Organization and application info must be set. Otherwise one can't access
	// QSettings from all over the app. Change this to match the bots credentials.
	QCoreApplication::setOrganizationName("BotFatherProject");
	QCoreApplication::setOrganizationDomain("botfather.io");
	QCoreApplication::setApplicationName("Botfather");
	QCoreApplication::setApplicationVersion("1.0.0");
	QSettings::setDefaultFormat(QSettings::IniFormat);

	// Seed the random function once using the current time in msec as seed.
	qsrand(QTime::currentTime().msec());

	// Initialuse the browser so that it's one may connect to it's signals and
	// slots and use it's methods.
#if defined(_WIN32) || defined(_WIN64)
	Browser::init(__argc, __argv);
#else
	Browser::init(argc, argv);
#endif

	UpdateWidget update_widget;
	AuthWindow auth_window("botfather-be", QCoreApplication::applicationVersion(), "WmXrhd3ifA8MwTRsjFgkbVsVbGsSiYr4");
	ControlWindow control_window;
	
	// Make the auth window show when the update dialog is done.
	QObject::connect(&update_widget, SIGNAL(finished()), &auth_window, SLOT(show()));
	
	// Make the control window show when the auth window permits it.
	QObject::connect(&auth_window, &AuthWindow::permitted, &control_window, &ControlWindow::open);
	
	// Show the update dialog.
	update_widget.show();
	update_widget.checkForUpdates();
	
	// Runs the QApplication event loop blocking. When the event loop stops the timer
	// powering the CEF event loop will stop aswell. After that the CEF can be shut
	// down calling Browser::quit(). Subsequently both cef and qt event and message loops
	// stopped, the applications exit code can be returned.
	int return_code = a.exec();
	Browser::quit();
	return return_code;
}