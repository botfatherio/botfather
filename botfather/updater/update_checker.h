#ifndef BFP__UPDATER__UPDATE_CHECKER_H
#define BFP__UPDATER__UPDATE_CHECKER_H

#include <QObject>
#include <QProcess>

class UpdateChecker : public QObject
{
	Q_OBJECT
public:
	explicit UpdateChecker(QObject *parent = nullptr);
	enum class ErrorType{
		MTOOL_START_FAILED,
		MTOOL_CRASHED,
		UNKNOWN_MTOOL_ERROR,
		CANT_CONNECT_TO_REPO
	};
	static QString maintainancetoolPath();
	
signals:
	void updatesAvailable();
	void noUpdatesAvailable();
	void updateCheckFailed(UpdateChecker::ErrorType error_type);
	
public slots:
	void checkForUpdates();
	void cancelUpdateCheck();
	void resultsAvailable(int, QProcess::ExitStatus);
	
private:
	QProcess *maintenancetool_process;
};

#endif // BFP__UPDATER__UPDATE_CHECKER_H