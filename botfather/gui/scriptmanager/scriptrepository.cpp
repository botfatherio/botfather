#include "scriptrepository.h"
#include <QFileInfo>
#include <QUrl>

#include <QDebug>
#include <git2.h> // TODO: move git functionality somewhere else

ScriptRepository::ScriptRepository(QObject *parent)
	: QObject(parent)
{

}

ScriptRepository::ScriptRepository(ScriptRepository::Data data, QObject *parent)
	: QObject(parent)
	, m_data(data)
{

}

ScriptRepository::ScriptRepository(const QString name, const QString developer, const QString description, const QString repository, QObject *parent)
	: QObject(parent)
	, m_data(name, developer, description, repository)
{

}

bool ScriptRepository::isValid() const
{
	return isLocal() || isRemote();
}

bool ScriptRepository::isLocal() const
{
	return !repository().isEmpty() && QFileInfo(repository()).isFile();
}

bool ScriptRepository::isRemote() const
{
	return !repository().isEmpty() && QUrl(repository()).isValid();
}

ScriptRepository::Status ScriptRepository::status() const
{
	return Status::Unavailabe;
}

ScriptRepository::Data ScriptRepository::data() const
{
	return m_data;
}

QString ScriptRepository::name() const
{
	return m_data.name;
}

void ScriptRepository::setName(const QString &name)
{
	m_data.name = name;
}

QString ScriptRepository::developer() const
{
	return m_data.developer;
}

void ScriptRepository::setDeveloper(const QString &developer)
{
	m_data.developer = developer;
}

QString ScriptRepository::description() const
{
	return m_data.description;
}

void ScriptRepository::setDescription(const QString &description)
{
	m_data.description = description;
}

QString ScriptRepository::repository() const
{
	return m_data.repository;
}

void ScriptRepository::setRepository(const QString &repository)
{
	m_data.repository = repository;
}
