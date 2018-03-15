#include "update_checker.h"
#include <QApplication>
#include <QDebug>

UpdateChecker::UpdateChecker(QObject *parent) : QObject(parent)
{
	maintenancetool_process = new QProcess(this);
	connect(maintenancetool_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(resultsAvailable(int,QProcess::ExitStatus)));
	//connect(maintenancetool_process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SIGNAL(updateCheckFailed(QProcess::ProcessError)));
}

QString UpdateChecker::maintainancetoolPath()
{
#if defined(Q_OS_WIN32)
	return QApplication::applicationDirPath() + "/maintenancetool.exe";
#elif defined(Q_OS_UNIX)
	return QApplication::applicationDirPath() + "/maintenancetool";
#else
	#error By UpdateChecker yet unsupported platform.
#endif
}

void UpdateChecker::checkForUpdates()
{
	QStringList arguments;
	arguments.append("--checkupdates");
	int code = maintenancetool_process->execute(maintainancetoolPath(), arguments);
	
	if (code == -2) {
		qDebug() << "Mtool start failed";
		emit updateCheckFailed(ErrorType::MTOOL_START_FAILED);
	}
	else if (code == -1) {
		qDebug() << "Mtool crashed.";
		emit updateCheckFailed(ErrorType::MTOOL_CRASHED);
	}
	else if (code != 0) {
		qDebug() << "Unknown mtool error. Exit code is" << code;
		emit updateCheckFailed(ErrorType::UNKNOWN_MTOOL_ERROR);
	}
}

void UpdateChecker::cancelUpdateCheck()
{
	if (!maintenancetool_process || maintenancetool_process->state() == QProcess::NotRunning) {
		return;
	}
	maintenancetool_process->kill();
	maintenancetool_process->waitForFinished(1000);
}

void UpdateChecker::resultsAvailable(int, QProcess::ExitStatus)
{
	QByteArray result = maintenancetool_process->readAllStandardOutput();
	if (result.contains("no updates available.")) {
		emit noUpdatesAvailable();
	}
	else if (result.contains("<updates>")) {
		emit updatesAvailable();
	}
	else {
		emit updateCheckFailed(ErrorType::CANT_CONNECT_TO_REPO);
	}
}