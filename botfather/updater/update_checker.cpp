#include "update_checker.h"
#include <QApplication>
#include <QDebug>

UpdateChecker::UpdateChecker(QObject *parent) : QObject(parent)
{
	qRegisterMetaType<UpdateChecker::ErrorType>("UpdateChecker::ErrorType");
	maintenancetool_process = new QProcess(this);
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

#include <QThread>

void UpdateChecker::checkForUpdates()
{
	
	QThread::sleep(5);
	
	QStringList arguments("--checkupdates");
	maintenancetool_process->start(maintainancetoolPath(), arguments);	
	maintenancetool_process->waitForFinished();
	
	// Why not async? Because process never emits finished if the binary can't be found.
	
	QByteArray output = maintenancetool_process->readAllStandardOutput();
	QByteArray errors = maintenancetool_process->readAllStandardError();
	QProcess::ProcessError process_error = maintenancetool_process->error();

	switch (process_error) {
	case QProcess::ProcessError::FailedToStart:
		emit updateCheckFailed(UpdateChecker::ErrorType::MTOOL_FAILED_TO_START);
		return;
	case QProcess::ProcessError::Crashed:
		emit updateCheckFailed(UpdateChecker::ErrorType::MTOOL_CRASHED);
		return;
	case QProcess::ProcessError::Timedout:
		emit updateCheckFailed(UpdateChecker::ErrorType::MTOOL_TIMEDOUT);
		return;
	case QProcess::ProcessError::WriteError:
		emit updateCheckFailed(UpdateChecker::ErrorType::MTOOL_WRITEERROR);
		return;
	case QProcess::ProcessError::ReadError:
		emit updateCheckFailed(UpdateChecker::ErrorType::MTOOL_READERROR);
		return;
	default:
		// QProcess::ProcessError::UnknownError is the default return value of
		// QProcess::error(). Thus we can't really thread it as an error.
		break;
	}
	
	//qDebug() << "output" << output;
	//qDebug() << "errors" << errors;
	//qDebug() << "process_error" << process_error;
	
	if (output.isEmpty() && errors.isEmpty()) {
		emit updateCheckFailed(UpdateChecker::ErrorType::UNKNOWN_ERROR);
		return;
	}
	
	else if (errors.contains("no updates available")) {
		emit noUpdatesAvailable();
		return;
	}
	
	else if (!errors.isEmpty()) {
		emit updateCheckFailed(UpdateChecker::ErrorType::REPO_NETWORK_ERROR);
		return;
	}

	// errors empty, output not
	emit updatesAvailable();
}

void UpdateChecker::cancelUpdateCheck()
{
	if (!maintenancetool_process || maintenancetool_process->state() == QProcess::NotRunning) {
		return;
	}
	maintenancetool_process->kill();
	maintenancetool_process->waitForFinished(1000);
}