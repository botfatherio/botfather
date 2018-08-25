#ifndef BFP_ENGINE_APIS_HELPER_API_H
#define BFP_ENGINE_APIS_HELPER_API_H

#include <QObject>
#include <QScriptEngine>
#include <QString>
#include "abstract_api.h"

class HelperAPI : public AbstractAPI
{
	Q_OBJECT
	
public:
	using AbstractAPI::AbstractAPI;
	
	// Makes the bot sleep for x seconds.
	Q_INVOKABLE void sleep(int seconds);
	
	// Makes the bot sleep for x milliseconds.
	Q_INVOKABLE void msleep(int milliseconds);
	
	// Returns the major part of the programs version.
	Q_INVOKABLE int getMajorVersion();
	
	// Returns the minor part of the programs version.
	Q_INVOKABLE int getMinorVersion();
	
	// Returns the path part of the programs version.
	Q_INVOKABLE int getPatchVersion();

	// Returns a absolute path to the dir of the currently run script.
	Q_INVOKABLE QString getAbsoluteScriptDirPath();
	
	// Plays the provided wav sound either blocking or not.
	Q_INVOKABLE void playWavSound(QString path_to_wav_file, bool blocking = true);
	
	// Stops any started wav sounds started by the bot from playing.
	Q_INVOKABLE void stopWavSound();
	
	// Methods added via extendGlobalApiObject():
	// Helper.log(comma, seperated, QScriptValues, aka, the, message)
	
protected:
	static QScriptValue testlog(QScriptContext *context, QScriptEngine *engine);
	void extendGlobalApiObject(QScriptEngine *engine, QScriptValue &api_object);
};

#endif // BFP_ENGINE_APIS_HELPER_API_H