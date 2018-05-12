#ifndef BFP__SCRIPTING__HELPER_API_H
#define BFP__SCRIPTING__HELPER_API_H

#include <QObject>
#include <QJSEngine>
#include <QString>

class Bot;
class BotThread;

class HelperAPI : public QObject
{
	Q_OBJECT
	
public:
	HelperAPI(Bot* bot_p, BotThread* bot_thread_p, QJSEngine* engine_p);
	static void enable(Bot* bot_p, BotThread* bot_thread_p, QJSEngine* engine_p);
	
	// Returns true if the file exists.
	Q_INVOKABLE bool fileExists(QString file_path);
	
	// Makes the bot sleep for x seconds.
	Q_INVOKABLE void sleep(int seconds);
	
	// Makes the bot sleep for x milliseconds.
	Q_INVOKABLE void msleep(int milliseconds);
	
	// Returns the applications version number.
	Q_INVOKABLE QString getVersion();

	// Returns the programs mode (gui, console).
	Q_INVOKABLE QString getClientMode();
	
	// Logs the message in the users chat.
	Q_INVOKABLE void log(QString log_message);
	
	// Returns false if the user requested the script to end.
	Q_INVOKABLE bool stopRequested();
	
	// Returns a absolute path to the dir of the currently run script.
	Q_INVOKABLE QString getAbsoluteScriptDirPath();
	
	Q_INVOKABLE void playWavSound(QString path_to_wav_file);

private:
	BotThread* m_bot_thread_p;
	Bot* m_bot_p;
	QJSEngine* m_engine_p;
};

#endif // BFP__SCRIPTING__HELPER_API_H