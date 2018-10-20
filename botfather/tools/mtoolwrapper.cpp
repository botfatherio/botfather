#include "mtoolwrapper.h"
#include <QStandardPaths>
#include <QProcess>

MtoolWrapper::MtoolWrapper(QObject *parent) : QObject(parent)
{

}

void MtoolWrapper::findExecutable()
{
	if (!getExecutablePath().isEmpty())
	{
		emit executableFound(true);
	}
}

void MtoolWrapper::startDetached(Mode mode)
{
	QString mtool = getExecutablePath();
	if (mtool.isEmpty()) return;

	QStringList arguments;

	switch (mode) {
	case Updater:
		arguments << "--updater";
		break;
	}

	if (QProcess::startDetached(mtool, arguments))
	{
		emit startedDetached();
	}
}

void MtoolWrapper::startDetachedUpdater()
{
	startDetached(Updater);
}

QString MtoolWrapper::getExecutablePath()
{
	return QStandardPaths::findExecutable("maintenancetool", {"./"});
}
