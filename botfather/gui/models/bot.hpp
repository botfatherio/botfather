#ifndef BOT_H
#define BOT_H

#include <QObject>
#include <QThread>

#include "../../engine/engine.hpp"
#include "bot_manifest.hpp"

class Bot : public QObject {
    Q_OBJECT

   public:
    // We use a struct to serialize a bots data because QObjects can't be
    // serialized directly.
    struct Data {
        QString path;  // Use the path to identify this bot
        QString name;
        QString repo;  // BF ratings are bind to a repository, we don't want it
                       // to change.

        Data() {}
        Data(const QString &path, const QString &name, const QString &repo) {
            this->path = path;
            this->name = name;
            this->repo = repo;
        }
    };

    enum class Status {
        Unavailabe,
        UpToDate,
        Outdated,
    };

    explicit Bot(QObject *parent = nullptr);
    explicit Bot(const Bot::Data &data, QObject *parent = nullptr);

    // Returns true if the bots name and path are empty.
    bool isNull() const;

    // Returns true if the bot isNull returns false and the bots path exists.
    bool isValid() const;

    bool isRunning() const;

    // Whether this bot is technically updateable. It is if it has a valid git
    // repository.
    bool isUpdatable() const;

    Bot::Data data() const;

    // Whether the bot is uptodate with its remote git repo (if there is such)
    Status status() const;
    void setStatus(const Status &status);

    // The path to the folder in which the bots files are stored. Use a bots
    // path to identify it.
    QString path() const;
    void setPath(const QString &path);

    // The name given to the bot by the user.
    QString name() const;
    void setName(const QString &name);

    // The scripts remote git repository url. Returns an empty string if the bot
    // is local (has no remote repo).
    QString repo() const;
    void setRepo(const QString &repo);

    // Returns the path to the first script file found in the bots directory or
    // an empty string if nothing has been found.
    QString scriptPath() const;

    // Returns the path to the bots settings such as hotkeys and wether to use
    // the debug mode
    QString settingsPath() const;

    // Returns the path to the bots script configuration set by the user
    QString configPath() const;

    // Returns the path to the bots manifest
    QString manifestPath() const;

    // Returns the bots loaded manifest if such exists
    BotManifest manifest() const;

    // Returns true when the bots files have been successfully deleted
    bool deleteFiles();

   public slots:
    void start();
    void stop();

    void checkStatus();
    void noteDifferencesToRemote(int differences_to_remote);

   signals:
    void nameChanged(const QString &new_name);
    void log(const QString &message, const Engine::LogSource &source);
    void audioPlaybackRequested(const QString &path_to_wav_file);
    void audioStopRequested();
    void statsUpdated(const QString &group, const QString &label,
                      const QString &value);
    void statsCleared();

    void started();
    void stopped();

    void statusChanged(const Status &status);
    void statusCheckFailed();

   private:
    Status m_status = Status::Unavailabe;
    Bot::Data m_data;
    bool m_is_running = false;
    Engine *m_engine;
    QThread *m_engine_thread;
};

Q_DECLARE_METATYPE(Bot *)

inline QDataStream &operator<<(QDataStream &stream, const Bot::Data &data) {
    return stream << data.name << data.path << data.repo;
}

inline QDataStream &operator>>(QDataStream &stream, Bot::Data &data) {
    return stream >> data.name >> data.path >> data.repo;
}

#endif  // BOT_H
