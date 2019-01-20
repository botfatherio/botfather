#include "bot.h"
#include <QDir>

Bot::Bot(QObject *parent) : QObject(parent)
{

}

Bot::Bot(const Bot::Data &data, QObject *parent)
	: QObject(parent)
	, m_data(data)
{

}

bool Bot::isNull() const
{
	return m_data.name.isEmpty() && m_data.path.isEmpty();
}

bool Bot::isValid() const
{
	return !isNull() && QDir(m_data.path).exists();
}

Bot::Data Bot::data() const
{
	return m_data;
}

void Bot::setData(const Bot::Data &data)
{
	m_data = data;
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
}

QString Bot::repo() const
{
	return m_data.repo;
}

void Bot::setRepo(const QString &repo)
{
	m_data.repo = repo;
}

