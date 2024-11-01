#ifndef BFP_ENGINE_APIS_HELPER_API_H
#define BFP_ENGINE_APIS_HELPER_API_H

#include <QObject>
#include <QScriptEngine>
#include <QString>

#include "abstract_api.hpp"

class HelperAPI : public AbstractAPI {
    Q_OBJECT

   public:
    using AbstractAPI::AbstractAPI;

    // Makes the bot sleep for x seconds.
    Q_INVOKABLE void sleep(int seconds);

    // Makes the bot sleep for x milliseconds.
    Q_INVOKABLE void msleep(int milliseconds);

    // Plays the provided wav sound either blocking or not.
    Q_INVOKABLE void playWavSound(const QString &path_to_wav_file);

    // Stops any started wav sounds started by the bot from playing.
    Q_INVOKABLE void stopWavSound();

    // Methods added via extendGlobalApiObject():
    // Helper.log(comma, separated, QScriptValues, aka, messages);
    // Helper.debug(comma, separated, QScriptValues, aka, messages);

   protected:
    static QStringList qScriptValueToStringList(const QScriptValue &value,
                                                bool quote_strings = false);
    static QScriptValue genericLog(QScriptContext *context,
                                   QScriptEngine *engine,
                                   Engine::LogSource source);
    static QScriptValue log(QScriptContext *context, QScriptEngine *engine);
    static QScriptValue debug(QScriptContext *context, QScriptEngine *engine);
    void extendGlobalApiObject(QScriptEngine *engine, QScriptValue &api_object);
};

#endif  // BFP_ENGINE_APIS_HELPER_API_H
