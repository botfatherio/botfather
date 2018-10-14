#include "helper_api.h"
#include <QFileInfo>
#include <QThread>
#include <QInputDialog>
#include <QAudioBuffer>
#include <QDebug>

void HelperAPI::sleep(int seconds)
{
	if (seconds < 1) {
		engine()->currentContext()->throwError(QScriptContext::RangeError, "seconds must be at least 1 second.");
		return void();
	}
	// Not sleeping for at least one ms can cause trouble. Better don't stop sleeping when stop is
	// requested to prevent bugs. Par example:
	// Stopping a script having an infinit loop with sleep in it will result in the bot going crazy.
	// CPU usage rockets up and one can no longer stop nor kill the script (because it will never sleep).
	QThread::sleep(static_cast<unsigned int>(seconds));
}

void HelperAPI::msleep(int milliseconds)
{
	if (milliseconds < 1) {
		engine()->currentContext()->throwError(QScriptContext::RangeError, "milliseconds must be at least 1 millisecond.");
		return void();
	}
	QThread::msleep(static_cast<unsigned long>(milliseconds));
}

void HelperAPI::playWavSound(const QString &path_to_wav_file)
{
	if (!bot()->scriptFileExists(path_to_wav_file)) {
		engine()->currentContext()->throwError(QScriptContext::TypeError, "path_to_wav_file does not exist.");
		return void();
	}
	emit bot()->playWavSound(bot()->normalisePath(path_to_wav_file));
}

void HelperAPI::stopWavSound()
{
	emit bot()->stopWavSound();
}

QScriptValue HelperAPI::log(QScriptContext *context, QScriptEngine *engine)
{
	QString message;
	
    for (int i = 0; i < context->argumentCount(); ++i) {
		if (i > 0) {
            message.append(" ");
		}
        message.append(context->argument(i).toString());
    }
	
	QScriptValue callee_data(context->callee().data());
	HelperAPI *hapi = qobject_cast<HelperAPI*>(callee_data.toQObject());
	emit hapi->bot()->message(message, false);

	// NOTE: Do not delete HelperAPI pointer here. This would delete the original helperapi instance,
	// which could no longer be used in the script engine.
	//delete hapi;

    return engine->undefinedValue();
}

void HelperAPI::extendGlobalApiObject(QScriptEngine *engine, QScriptValue &api_object)
{
	QScriptValue fun(engine->newFunction(log));
	fun.setData(engine->newQObject(this));
	api_object.setProperty("log", fun);
}
