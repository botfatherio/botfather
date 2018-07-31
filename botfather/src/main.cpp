#include <QApplication>
#include <QSettings>
#include <QTime>
#include <QStandardPaths>
#include <QProcess>
#include <QFileInfo>
#include <QMessageBox>
#include "gui/control_window.h"
#include "browser/browser.h"
#include "gui/status_dialog.h"

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

#ifdef Q_OS_LINUX
	// On linux the desktop api needs permission to write to /dev/uinput to generate
	// authentic not ignore input events. On Ubuntu the file is writable (660). On other
	// systems it might not be, so we must check that.	
	
	QFileInfo fi("/dev/uinput");
	if (!fi.isWritable()) {
		QString pkexec_path = QStandardPaths::findExecutable("pkexec", {"/usr/bin/"});
		
		QMessageBox *box = new QMessageBox;
		box->setWindowTitle("Permissions required");
		box->setWindowIcon(QIcon(":/gui/logo_v2.png"));
		box->setIcon(QMessageBox::Information);
		box->setStandardButtons(QMessageBox::Ignore | QMessageBox::Ok);
		box->setDefaultButton(QMessageBox::Ok);
		
		box->setDetailedText(
			"Botfathers Desktop API uses the uinput linux kernel module to generate keyboard and mouse input events. "
			"Thus botfather requires write permission on the '/dev/uinput' file.\n"
			"On some distributions that file is writable by default. On your machine it's currently not writable. "
			"The command 'chmod 660 /dev/uinput' makes it writable.\n"
			"When 'pkexec' is installed on your system botfather can execute the command for you. "
			"Otherwise you have to execute it manually from a command line.\n"
			"If you don't want to run scripts controlling your desktop anyway, you don't have to make that file writable."
		);
		
		if (pkexec_path.isEmpty()) {
			box->setText(
				"Botfather requires write permissions on '/dev/uinput' to fake keyboard and mouse input events.\n"
				"Please run 'sudo chmod 660 /dev/uinput' to grant the required permissions."
			);
		} else {
			box->setText(
				"Botfather requires write permissions on '/dev/uinput' to fake keyboard and mouse input events.\n"
				"Click 'Ok' to grant the required permissions."
			);
		}
		
		if (box->exec() == QMessageBox::Ok && !pkexec_path.isEmpty()) {
			QProcess *p = new QProcess;
			p->start(pkexec_path, {"chmod", "660", "/dev/uinput"});
			p->waitForFinished();
		}
	}
#endif
	
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