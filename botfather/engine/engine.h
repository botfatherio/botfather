#ifndef BFP_ENGINE_ENGINE_H
#define BFP_ENGINE_ENGINE_H

#include <QObject>
#include <QString>
#include <QScriptEngine>
#include <QMetaType>

class Engine : public QObject
{
	Q_OBJECT

public:
	// Giving Bot a parent results in: QObject::moveToThread: Cannot move objects with a parent
	Engine(QString script_path);
	~Engine();

	enum LogSource{
		System,
		Error,
		Script,
		Debug
	};
	
	// Considers relative paths relative to the scripts dir and makes them absolute.
	QString normalisePath(QString path);
	
	QString getAbsoluteScriptDirPath();
	bool scriptFileExists(QString file_path);
	
public slots:
	void runScript();
	void stop();

signals:
	void started();
	void stopped(bool without_errors);
	void log(const QString &message, const Engine::LogSource &source);
	
	// Start/stop a wav sound in the main thread.
	void playWavSound(QString path_to_wav_file);
	void stopWavSound();
	
protected:
	static QString replaceQtWithEngineTypeNames(QString text);
	
private:
	QScriptEngine *script_engine;
	QString script_path;
};

#endif // BFP_ENGINE_ENGINE_H
