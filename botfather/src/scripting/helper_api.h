#ifndef BFP__SCRIPTING__HELPER_API_H
#define BFP__SCRIPTING__HELPER_API_H

#include <QObject>
#include <QScriptEngine>
#include <QString>

class Bot;
class BotThread;

class HelperAPI : public QObject
{
	Q_OBJECT
	
public:
	HelperAPI(Bot* bot_p, BotThread* bot_thread_p, QScriptEngine* engine_p);
	static void enable(Bot* bot_p, BotThread* bot_thread_p, QScriptEngine* engine_p);
	
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
	
	// Plays the provided wav sound either blocking or not.
	Q_INVOKABLE void playWavSound(QString path_to_wav_file, bool blocking = true);
	
	// Stops any started wav sounds started by the bot from playing.
	Q_INVOKABLE void stopWavSound();

private:
	BotThread* m_bot_thread_p;
	Bot* m_bot_p;
	QScriptEngine* m_engine_p;
};

#endif // BFP__SCRIPTING__HELPER_API_H