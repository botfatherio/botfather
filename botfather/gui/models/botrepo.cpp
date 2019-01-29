#include "botrepo.h"
#include <QUrl>
#include <QDebug>

BotRepo::BotRepo(const QString &name, const QString &developer, const QString &description, const QString &git_url)
	: m_name(name)
	, m_developer(developer)
	, m_description(description)
	, m_git_url(git_url)
{

}

bool BotRepo::isValid() const
{
	return !gitUrl().isEmpty() && QUrl(gitUrl()).isValid();
}

QString BotRepo::name() const
{
	return m_name;
}

void BotRepo::setName(const QString &name)
{
	m_name = name;
}

QString BotRepo::developer() const
{
	return m_developer;
}

void BotRepo::setDeveloper(const QString &developer)
{
	m_developer = developer;
}

QString BotRepo::description() const
{
	return m_description;
}

void BotRepo::setDescription(const QString &description)
{
	m_description = description;
}

QString BotRepo::gitUrl() const
{
	return m_git_url;
}

void BotRepo::setGitUrl(const QString &git_url)
{
	m_git_url = git_url;
}
