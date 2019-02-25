#include "maintenancetool.h"
#include <QStandardPaths>
#include <QDebug>

MaintenanceTool::MaintenanceTool(QObject *parent)
	: QObject(parent)
{

}

QString MaintenanceTool::filePath()
{
	return QStandardPaths::findExecutable("maintenancetool", {"./"});
}

bool MaintenanceTool::exists()
{
	return !filePath().isEmpty();
}

void MaintenanceTool::checkForUpdates()
{
	QProcess *process = getPreparedProcess();
	connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [this, process](int, QProcess::ExitStatus exit_status)
	{
		if (exit_status != QProcess::ExitStatus::NormalExit)
		{
			emit errorsOccured();
		}
		else if (process->readAllStandardOutput().contains("<updates>"))
		{
			emit updatesAvailable();
		}
	});
	process->start(filePath(), {"--checkupdates"});
}

void MaintenanceTool::startDetachedAsUpdater()
{
	if (getPreparedProcess()->startDetached(filePath(), {"--updater"}))
	{
		emit updaterStarted();
	}
}

QProcess *MaintenanceTool::getPreparedProcess()
{
	QProcess *process = new QProcess(this);
	connect(process, &QProcess::started, this, &MaintenanceTool::started);
	connect(process, &QProcess::errorOccurred, this, &MaintenanceTool::errorsOccured);
	return process;
}
