#include "scriptrepository.h"
#include <QFileInfo>
#include <QUrl>

ScriptRepository::ScriptRepository()
{

}

ScriptRepository::ScriptRepository(const QString name, const QString developer, const QString description, const QString repository)
	: m_name(name)
	, m_developer(developer)
	, m_description(description)
	, m_repository(repository)
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

QString ScriptRepository::name() const
{
	return m_name;
}

void ScriptRepository::setName(const QString &name)
{
	m_name = name;
}

QString ScriptRepository::developer() const
{
	return m_developer;
}

void ScriptRepository::setDeveloper(const QString &developer)
{
	m_developer = developer;
}

QString ScriptRepository::description() const
{
	return m_description;
}

void ScriptRepository::setDescription(const QString &description)
{
	m_description = description;
}

QString ScriptRepository::repository() const
{
	return m_repository;
}

void ScriptRepository::setRepository(const QString &repository)
{
	m_repository = repository;
}
