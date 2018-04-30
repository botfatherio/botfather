#include <QApplication>
#include <QSettings>
#include <QTime>
#include "gui/auth_dialog.h"
#include "gui/control_window.h"
#include "browser/browser.h"
#include "updater/updater_dialog.h"

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
	QCoreApplication::setApplicationVersion("2.2.0");
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

	UpdaterDialog *updater_dialog = new UpdaterDialog;
	updater_dialog->show();
	
	// We can't execute the UpdaterDialog, otherwise using QApplication::quit() would work, but the UpdaterDialog
	// needs to make use of it. Thus we have to use show(), but we dont won't to see the auth and control window
	// while the updater is visible. Thats why we show the laters ones not until the UpdaterDialog finished.
	QObject::connect(updater_dialog, &UpdaterDialog::finished, []() {
		ControlWindow *control_window = new ControlWindow;
		AuthDialog *auth_dialog = new AuthDialog("botfather", QCoreApplication::applicationVersion(), "ABd7qdoggCN6nZvj6eL3Ndwoc6azL8SD", control_window);
		
		// Tell the control window about the user license and the programs stability.
		QObject::connect(auth_dialog, SIGNAL(authenticated(int,int,bool)), control_window, SLOT(applyRemoteApiInfo(int,int,bool)));
		
		control_window->show();
		auth_dialog->tryAutoLogin(); // If AutoLogin doesn't work, the auth dialog will be presented to the user.
	});
	
	// Runs the QApplication event loop blocking. When the event loop stops the timer
	// powering the CEF event loop will stop aswell. After that the CEF can be shut
	// down calling Browser::quit(). Subsequently both cef and qt event and message loops
	// stopped, the applications exit code can be returned.
	int return_code = a.exec();
	Browser::quit();
	return return_code;
}