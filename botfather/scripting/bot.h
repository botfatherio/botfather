#ifndef BFB__SCRIPTING__BOT_H
#define BFB__SCRIPTING__BOT_H

#include <QObject>
#include <QString>

class Bot : public QObject
{
	Q_OBJECT

public:
	Bot(QString script_path);
	
	// Returns true if the file exists.
	Q_INVOKABLE static bool fileExists(QString file_path);
	
	// Makes the bot sleep for x seconds.
	Q_INVOKABLE static void sleep(int seconds);
	
	// Makes the bot sleep for x milliseconds.
	Q_INVOKABLE static void msleep(int milliseconds);
	
	// Returns the client major version number.
	Q_INVOKABLE static int getVersionMajor();
	
	// Returns the client minor version number.
	Q_INVOKABLE static int getVersionMinor();
	
	// Returns the client path version number.
	Q_INVOKABLE static int getVersionPatch();
	
	// Returns the botfathers client type (browser, desktop, android).
	Q_INVOKABLE static QString getClientType();
	
	// Returns the programs mode (gui, console).
	Q_INVOKABLE static QString getClientMode();
	
	// Logs the message in the users chat.
	Q_INVOKABLE void log(QString log_message);
	
public slots:
	void runScript();

signals:
	void started();
	void stopped(bool without_errors);
	void message(QString log_message);
	
private:
	QString m_script_path;
};

#endif // BFB__SCRIPTING__BOT_H