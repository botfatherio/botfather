#include "bot.hpp"

#include <git2.h>

#include <QDir>

#include "../../git/git_behind_operation.hpp"
#include "../../git/git_fetch_operation.hpp"

Bot::Bot(QObject *parent) : QObject(parent) {}

Bot::Bot(const QString &path, const QString &name, const QString &repo,
         const QString &branch, QObject *parent)
    : Bot(parent) {
    m_path = path;
    m_name = name;
    m_repo = repo;
    m_branch = branch;
}

bool Bot::isNull() const { return m_name.isEmpty() && m_path.isEmpty(); }

bool Bot::isValid() const { return !isNull() && QDir(m_path).exists(); }

bool Bot::isRunning() const { return m_is_running; }

bool Bot::isUpdatable() const {
    // Pass nullptr for the output parameter to check for but not open the repo
    return isValid() &&
           git_repository_open_ext(nullptr, path().toUtf8(),
                                   GIT_REPOSITORY_OPEN_NO_SEARCH, nullptr) == 0;
}

Bot::Status Bot::status() const { return m_status; }

void Bot::setStatus(const Status &status) {
    m_status = status;
    emit statusChanged(status);
}

QString Bot::path() const { return m_path; }

void Bot::setPath(const QString &path) { m_path = path; }

QString Bot::name() const { return m_name; }

void Bot::setName(const QString &name) {
    m_name = name;
    emit nameChanged(name);
}

QString Bot::repo() const { return m_repo; }

void Bot::setRepo(const QString &repo) { m_repo = repo; }

QString Bot::branch() const { return m_branch; }

void Bot::setBranch(const QString &branch) { m_branch = branch; }

QString Bot::scriptPath() const {
    QDir repo_dir(path());
    if (!isValid() || repo_dir.isEmpty()) {
        return QString();  // Invalid or empty bot directory
    }

    QFileInfo manifested_script(repo_dir.filePath(manifest().scriptPath()));
    if (manifested_script.exists() && manifested_script.isFile()) {
        // The script Engine will complain by itself if the manifested file is
        // not a script
        return manifested_script.absoluteFilePath();
    }

    QStringList scriptname_filters;
    scriptname_filters << "*.js";

    // Don't check for readability yet, the bot engine informs the user about
    // permission issues
    QStringList entries = repo_dir.entryList(
        scriptname_filters, QDir::Files | QDir::NoDotAndDotDot);

    if (entries.isEmpty()) {
        return QString();  // No script found
    }

    return repo_dir.filePath(entries.first());
}

QString Bot::settingsPath() const {
    return path().isEmpty() ? QString()
                            : path() + QDir::separator() + ".settings.ini";
}

QString Bot::configPath() const {
    QDir bot_dir(path());
    return bot_dir.exists() ? bot_dir.filePath(".config.json") : QString();
}

QString Bot::manifestPath() const {
    QDir bot_dir(path());
    return bot_dir.exists() ? bot_dir.filePath("manifest.json") : QString();
}

BotManifest Bot::manifest() const {
    BotManifest bot_manifest(configPath());
    bot_manifest.loadFromFile(manifestPath());
    return bot_manifest;
}

bool Bot::deleteFiles() {
    QDir bot_dir(path());
    if (bot_dir.removeRecursively()) {
        setPath(QString());  // Clear the bots path and thus render it invalid
        return true;
    }
    return false;
}

void Bot::start() {
    if (isRunning()) return;

    m_engine_thread = new QThread;
    m_engine_thread->setObjectName("Engine thread for " + name());

    m_engine = new Engine(scriptPath(), scriptPath(), configPath());
    m_engine->moveToThread(m_engine_thread);

    connect(m_engine_thread, &QThread::started, m_engine, &Engine::runScript);
    connect(m_engine, &Engine::stopped, m_engine_thread, &QThread::quit);
    connect(m_engine, &Engine::stopped, m_engine, &QObject::deleteLater);
    connect(m_engine_thread, &QThread::finished, m_engine_thread,
            &QObject::deleteLater);

    connect(m_engine_thread, &QThread::finished, [this]() {
        m_is_running = false;
        emit stopped();
    });

    connect(m_engine, &Engine::log, this, &Bot::log);
    connect(m_engine, &Engine::statsUpdated, this, &Bot::statsUpdated);
    connect(m_engine, &Engine::statsCleared, this, &Bot::statsCleared);
    connect(m_engine, &Engine::playWavSound, this,
            &Bot::audioPlaybackRequested);
    connect(m_engine, &Engine::stopWavSound, this, &Bot::audioStopRequested);

    m_engine_thread->start();
    m_is_running = true;
    emit started();
}

void Bot::stop() {
    if (!isRunning()) return;
    Q_ASSERT(m_engine);
    m_engine->stop();
}

void Bot::checkStatus() {
    // To check whether the script is outdated we first have to fetch from the
    // remote. After doing so we can calculate the differences to the remote.

    QThread *fetch_thread =
        new QThread;  // Don't give it a parent, otherwise the app will crash
                      // when the parent gets destroyed before the thread
                      // finished.
    fetch_thread->setObjectName(
        QString("GitFetchOperation Thread for %0").arg(path()));

    GitFetchOperation *fetch_op = new GitFetchOperation(path());
    fetch_op->moveToThread(fetch_thread);

    connect(fetch_thread, &QThread::started, fetch_op,
            &GitFetchOperation::process);
    connect(fetch_op, &GitFetchOperation::finished, fetch_thread,
            &QThread::quit);
    connect(fetch_op, &GitFetchOperation::finished, fetch_op,
            &GitFetchOperation::deleteLater);
    connect(fetch_thread, &QThread::finished, fetch_thread,
            &QThread::deleteLater);

    QThread *behind_thread =
        new QThread;  // Don't give it a parent, otherwise the app will crash
                      // when the parent gets destroyed before the thread
                      // finished.
    behind_thread->setObjectName(
        QString("GitBehindOperation Thread for %0").arg(path()));

    GitBehindOperation *behind_op = new GitBehindOperation(path());
    behind_op->moveToThread(behind_thread);

    connect(behind_thread, &QThread::started, behind_op,
            &GitBehindOperation::process);
    connect(behind_op, &GitBehindOperation::finished, behind_thread,
            &QThread::quit);
    connect(behind_op, &GitBehindOperation::finished, behind_op,
            &GitBehindOperation::deleteLater);
    connect(behind_thread, &QThread::finished, behind_thread,
            &QThread::deleteLater);

    connect(behind_op, &GitBehindOperation::differencesToRemote, this,
            &Bot::noteDifferencesToRemote);
    connect(fetch_op, SIGNAL(finished()), behind_thread, SLOT(start()));
    connect(fetch_op, &GitFetchOperation::failure, this,
            &Bot::statusCheckFailed);
    connect(fetch_op, &GitBehindOperation::failure, this,
            &Bot::statusCheckFailed);

    fetch_thread->start();
}

void Bot::noteDifferencesToRemote(int differences) {
    setStatus(differences > 0 ? Status::Outdated : Status::UpToDate);
}
