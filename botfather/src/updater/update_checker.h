#ifndef BFP__UPDATER__UPDATE_CHECKER_H
#define BFP__UPDATER__UPDATE_CHECKER_H

#include <QObject>
#include <QProcess>

class UpdateChecker : public QObject
{
	Q_OBJECT
public:
	explicit UpdateChecker(QObject *parent = nullptr);
	static QString maintainancetoolPath();
	
	enum class ErrorType{
		MTOOL_FAILED_TO_START,
		MTOOL_CRASHED,
		MTOOL_TIMEDOUT,
		MTOOL_WRITEERROR,
		MTOOL_READERROR,
		REPO_NETWORK_ERROR,
		UNKNOWN_ERROR
	};	
	
signals:
	void updatesAvailable();
	void noUpdatesAvailable();
	void updateCheckFailed(UpdateChecker::ErrorType error_type);
	
public slots:
	void checkForUpdates();
	void cancelUpdateCheck();
	
private:
	QProcess *maintenancetool_process;
};

// Declare the enum as metatype so it can be queued by the signal slot system.
Q_DECLARE_METATYPE(UpdateChecker::ErrorType)

#endif // BFP__UPDATER__UPDATE_CHECKER_H