#include "helper_api.hpp"

#include <QAudioBuffer>
#include <QDebug>
#include <QFileInfo>
#include <QInputDialog>
#include <QScriptValueIterator>
#include <QThread>

void HelperAPI::sleep(int seconds) {
    if (seconds < 1) {
        engine()->currentContext()->throwError(
            QScriptContext::RangeError, "seconds must be at least 1 second.");
        return void();
    }
    // Not sleeping for at least one ms can cause trouble. Better don't stop
    // sleeping when stop is requested to prevent bugs. Par example: Stopping a
    // script having an infinit loop with sleep in it will result in the bot
    // going crazy. CPU usage rockets up and one can no longer stop nor kill the
    // script (because it will never sleep).
    QThread::sleep(static_cast<unsigned int>(seconds));
}

void HelperAPI::msleep(int milliseconds) {
    if (milliseconds < 1) {
        engine()->currentContext()->throwError(
            QScriptContext::RangeError,
            "milliseconds must be at least 1 millisecond.");
        return void();
    }
    QThread::msleep(static_cast<unsigned long>(milliseconds));
}

void HelperAPI::playWavSound(const QString &path_to_wav_file) {
    if (!bot()->scriptFileExists(path_to_wav_file)) {
        engine()->currentContext()->throwError(
            QScriptContext::TypeError, "path_to_wav_file does not exist.");
        return void();
    }
    emit bot() -> playWavSound(bot()->normalisePath(path_to_wav_file));
}

void HelperAPI::stopWavSound() { emit bot() -> stopWavSound(); }

QStringList HelperAPI::qScriptValueToStringList(const QScriptValue &value,
                                                bool quote_strings) {
    QStringList strings;

    if (value.isArray()) {
        QScriptValueIterator value_it(value);
        QStringList array_strings;

        while (value_it.hasNext()) {
            value_it.next();
            if (value_it.hasNext())  // Prevent printing of the last array item,
                                     // which is its size
            {
                array_strings
                    << qScriptValueToStringList(value_it.value(), true);
            }
        }

        strings << "[" + array_strings.join(", ") + "]";
    }

    else if (value.isObject() && !value.property("toString").isFunction()) {
        // Print [object] if the object has no toString method to represent it
        // otherwise
        strings << "[object]";
    }

    else if (value.isString() && quote_strings) {
        strings << "\"" + value.toString() + "\"";
    }

    else {
        strings << value.toString();
    }

    return strings;
}

QScriptValue HelperAPI::genericLog(QScriptContext *context,
                                   QScriptEngine *engine,
                                   Engine::LogSource source) {
    QStringList strings;

    for (int i = 0; i < context->argumentCount(); ++i) {
        strings << qScriptValueToStringList(context->argument(i));
    }

    // We added a pointer to the HelperAPI instance when registering the log and
    // debug methods in HelperAPI::extendGlobalApiObject so that we can obtain
    // them here to get access to the HelperAPI from this static genericLog
    // function.

    QScriptValue callee_data(context->callee().data());
    HelperAPI *hapi = qobject_cast<HelperAPI *>(callee_data.toQObject());
    emit hapi->bot()->log(strings.join(" "), source);

    // NOTE: Do not delete HelperAPI pointer here. This would delete the
    // original helperapi instance, which could no longer be used by the script
    // engine.
    // delete hapi;

    return engine->undefinedValue();
}

QScriptValue HelperAPI::log(QScriptContext *context, QScriptEngine *engine) {
    return HelperAPI::genericLog(context, engine, Engine::LogSource::Script);
}

QScriptValue HelperAPI::debug(QScriptContext *context, QScriptEngine *engine) {
    return HelperAPI::genericLog(context, engine, Engine::LogSource::Debug);
}

void HelperAPI::extendGlobalApiObject(QScriptEngine *engine,
                                      QScriptValue &api_object) {
    QScriptValue log_function(engine->newFunction(log));
    QScriptValue debug_function(engine->newFunction(debug));

    // Add pointers to the HelperAPI instance as data to the methods.
    // This way we can obtain a pointer to the HelperAPI instance from
    // within the static HelperAPI::log and HelperAPI::debug functions.

    log_function.setData(engine->newQObject(this));
    debug_function.setData(engine->newQObject(this));

    api_object.setProperty("log", log_function);
    api_object.setProperty("debug", debug_function);
}
