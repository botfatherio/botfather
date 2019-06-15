#include "botrepo.hpp"
#include <QUrl>
#include <QDebug>

BotRepo::BotRepo(const QString &id, const QString &name, const QString &developer, const QString &description, const QString &platform, const QString &git_url)
	: m_id(id)
	, m_name(name)
	, m_developer(developer)
	, m_description(description)
	, m_platform(platform)
	, m_git_url(git_url)
{

}

bool BotRepo::isValid() const
{
	return !gitUrl().isEmpty() && QUrl(gitUrl()).isValid();
}

QString BotRepo::id() const
{
	return m_id;
}

void BotRepo::setId(const QString &id)
{
	m_id = id;
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

QString BotRepo::platform() const
{
	return m_platform;
}

void BotRepo::setPlatform(const QString &platform)
{
	m_platform = platform;
}

QString BotRepo::gitUrl() const
{
	return m_git_url;
}

void BotRepo::setGitUrl(const QString &git_url)
{
	m_git_url = git_url;
}

QString BotRepo::scriptUrl() const
{
	return QString("https://botfather.io/scripts/%1/").arg(id());
}
