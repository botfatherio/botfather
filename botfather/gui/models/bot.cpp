#include "bot.h"
#include <QDir>

Bot::Bot(QObject *parent) : QObject(parent)
{

}

Bot::Bot(const Bot::Data &data, QObject *parent) : Bot(parent)
{
	m_data = data;
}

bool Bot::isNull() const
{
	return m_data.name.isEmpty() && m_data.path.isEmpty();
}

bool Bot::isValid() const
{
	return !isNull() && QDir(m_data.path).exists();
}

bool Bot::isRunning() const
{
	return m_is_running;
}

Bot::Data Bot::data() const
{
	return m_data;
}

QString Bot::path() const
{
	return m_data.path;
}

void Bot::setPath(const QString &path)
{
	m_data.path = path;
}

QString Bot::name() const
{
	return m_data.name;
}

void Bot::setName(const QString &name)
{
	m_data.name = name;
	emit nameChanged(name);
}

QString Bot::repo() const
{
	return m_data.repo;
}

void Bot::setRepo(const QString &repo)
{
	m_data.repo = repo;
}

QString Bot::scriptPath() const
{
	QDir repo_dir(path());
	if (!isValid() || repo_dir.isEmpty())
	{
		return QString(); // Invalid or empty bot directory
	}

	QStringList scriptname_filters;
	scriptname_filters << "*.js";

	// Don't check for readability yet, the bot engine informs the user about permission issues
	QStringList entries = repo_dir.entryList(scriptname_filters, QDir::Files|QDir::NoDotAndDotDot);

	if (entries.isEmpty())
	{
		return QString(); // No script found
	}

	// TODO: extend to check in the scripts manifest for the main scriptfiles path.

	return repo_dir.filePath(entries.first());
}

QString Bot::settingsPath() const
{
	return path().isEmpty() ? QString() : path() + QDir::separator() + ".settings.ini";
}

void Bot::start()
{
	if (isRunning()) return;

	m_engine_thread = new QThread;
	m_engine_thread->setObjectName("Engine thread for " + name());

	m_engine = new Engine(scriptPath());
	m_engine->moveToThread(m_engine_thread);

	connect(m_engine_thread, &QThread::started, m_engine, &Engine::runScript);
	connect(m_engine, &Engine::stopped, m_engine_thread, &QThread::quit);
	connect(m_engine, &Engine::stopped, m_engine, &QObject::deleteLater);
	connect(m_engine_thread, &QThread::finished, m_engine_thread, &QObject::deleteLater);

	connect(m_engine_thread, &QThread::finished, [this](){
		m_is_running = false;
		emit stopped();
	});

	connect(m_engine, &Engine::log, this, &Bot::log);
	connect(m_engine, &Engine::playWavSound, this, &Bot::audioPlaybackRequested);
	connect(m_engine, &Engine::stopWavSound, this, &Bot::audioStopRequested);

	m_engine_thread->start();
	m_is_running = true;
	emit started();
}

void Bot::stop()
{
	if (!isRunning()) return;
	Q_ASSERT(m_engine);
	m_engine->stop();
}
