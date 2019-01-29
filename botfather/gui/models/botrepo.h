#ifndef BOT_REPO_H
#define BOT_REPO_H

#include <QString>
#include <QMetaType>

class BotRepo
{
public:
	BotRepo() {}
	BotRepo(const QString &name, const QString &developer, const QString &description, const QString &git_url);
	bool isValid() const;

	QString name() const;
	void setName(const QString &name);

	QString developer() const;
	void setDeveloper(const QString &developer);

	QString description() const;
	void setDescription(const QString &description);

	QString gitUrl() const;
	void setGitUrl(const QString &git_url);

private:
	QString m_name;
	QString m_developer;
	QString m_description;
	QString m_git_url;
};

Q_DECLARE_METATYPE(BotRepo)

#endif // BOT_REPO_H
