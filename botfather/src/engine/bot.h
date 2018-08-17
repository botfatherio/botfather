#ifndef BFB__SCRIPTING__BOT_H
#define BFB__SCRIPTING__BOT_H

#include <QObject>
#include <QString>
#include <QScriptEngine>

class Bot : public QObject
{
	Q_OBJECT

public:
	// Giving Bot a parent results in: QObject::moveToThread: Cannot move objects with a parent
	Bot(QString script_path);
	
public slots:
	void runScript();
	bool isRunning() const;
	void stop();
	
	// Makes a relative path to an absolute path relative to the script dir. Returns already
	// absolute paths as they are.
	QString normalisePath(QString path);
	
	QString getAbsoluteScriptDirPath();
	bool fileExists(QString file_path);
	
signals:
	void started();
	void stopped(bool without_errors);
	void message(QString log_message, bool from_client, bool error = false);
	
	// Start/stop a wav sound in the main thread.
	void playWavSound(QString path_to_wav_file);
	void stopWavSound();
	
private:
	QScriptEngine *script_engine;
	QString m_script_path;
	bool running = false;
};

#endif // BFB__SCRIPTING__BOT_H