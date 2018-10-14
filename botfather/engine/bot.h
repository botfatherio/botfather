#ifndef BFB_ENGINE_BOT_H
#define BFB_ENGINE_BOT_H

#include <QObject>
#include <QString>
#include <QScriptEngine>

class Bot : public QObject
{
	Q_OBJECT

public:
	// Giving Bot a parent results in: QObject::moveToThread: Cannot move objects with a parent
	Bot(QString script_path);
	~Bot();
	
	// Considers relative paths relative to the scripts dir and makes them absolute.
	QString normalisePath(QString path);
	
	QString getAbsoluteScriptDirPath();
	bool scriptFileExists(QString file_path);
	
public slots:
	void runScript();
	bool isRunning() const;
	void stop();

signals:
	void started();
	void stopped(bool without_errors);
	void message(QString log_message, bool from_client, bool error = false);
	
	// Start/stop a wav sound in the main thread.
	void playWavSound(QString path_to_wav_file);
	void stopWavSound();
	
protected:
	static QString replaceQtWithEngineTypeNames(QString text);
	
private:
	QScriptEngine *script_engine;
	QString script_path;
	bool running = false;
};

#endif // BFB_ENGINE_BOT_H
