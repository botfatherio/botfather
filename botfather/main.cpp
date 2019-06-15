#include <QApplication>
#include <QSettings>
#include <QTime>
#include <git2.h>
#include "shared/qsettingsjsonformat.hpp"
#include "engine/modules/browser/browser_manager.hpp"
#include "gui/mainwindow.hpp"

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

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
	QCoreApplication::setApplicationVersion("5.4.1");

	QSettingsJsonFormat::setFormat(QSettings::registerFormat("json", &QSettingsJsonFormat::readFunc, &QSettingsJsonFormat::writeFunc));
	QSettings::setDefaultFormat(QSettings::IniFormat);

#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
    // qrand needs to be seeded once per thread
    qsrand(static_cast<uint>(QTime::currentTime().msec()));
#endif

#if defined(_WIN32) || defined(_WIN64)
		Browser::init(__argc, __argv);
#else
		BrowserManager::instance()->init(argc, argv);
		BrowserManager::instance()->bind(&a);
#endif

    // Init libgit2 once and never shut it down. Shutting libgit2 down caused crashes related to multithreading
    // even when compiling libgit2 to be thread safe. The libgit2 docs state some interesting things:
    /* 1. (Thats a CMAKE flag ->) THREADSAFE. Selects whether libgit2 **tries** to be threadsafe
     * - https://libgit2.org/docs/guides/build-and-link/
     * 2. Usually you don’t need to call the shutdown function as the operating system will take care of reclaiming resources,
     * but if your application uses libgit2 in some areas which are not usually active, you can use
     * git_libgit2_shutdown();
     * to ask the library to clean up the global state. The cleanup will be performed once there have been the same number of calls
     * to git_libgit2_shutdown() as there were for git_libgit2_init().
     * - https://libgit2.org/docs/guides/101-samples/
     */
    git_libgit2_init();

	MainWindow *main_window = new MainWindow;
	main_window->show();

	// Runs the QApplication event loop blocking. When the event loop stops the timer
	// powering the CEF event loop will stop aswell. After that the CEF can be shut
	// down calling Browser::quit(). Subsequently both cef and qt event and message loops
	// stopped, the applications exit code can be returned.
	return a.exec();
}
