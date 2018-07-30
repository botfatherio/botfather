#include <QApplication>
#include <QSettings>
#include <QTime>
#include "gui/control_window.h"
#include "browser/browser.h"
//#include "updater/updater_dialog.h"

#include "gui/status_dialog.h"

#include <QStandardPaths>
#include <QProcess>



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
	QCoreApplication::setApplicationVersion("0.0.1");
	QSettings::setDefaultFormat(QSettings::IniFormat);

	// Seed the random function once using the current time in msec as seed.
	qsrand(static_cast<unsigned int>(QTime::currentTime().msec()));

	bool browser_initialized = false;
	StatusDialog *status_dialog = new StatusDialog();
	
	// Initialize the browser and start the controlwindow only when this version is supported
	QObject::connect(status_dialog, &StatusDialog::accepted, [&](){
	#if defined(_WIN32) || defined(_WIN64)
		Browser::init(__argc, __argv);
	#else
		Browser::init(argc, argv);
	#endif
		browser_initialized = true;
		
		ControlWindow *control_window = new ControlWindow;
		control_window->show();
	});

	// Quit the application if it's version is not supported. Exec returns 1 if the dialog has been accepted and 0 if rejected.
	// Rejected means our rest api told us this version is not supported.
	if (status_dialog->exec() == 0) {
		return 0;
	}

	// Runs the QApplication event loop blocking. When the event loop stops the timer
	// powering the CEF event loop will stop aswell. After that the CEF can be shut
	// down calling Browser::quit(). Subsequently both cef and qt event and message loops
	// stopped, the applications exit code can be returned.
	int return_code = a.exec();
	if (browser_initialized) {
		// Trying to quit the browser when it has not been initialized causes the client to crash
		Browser::quit();
	}
	return return_code;
}