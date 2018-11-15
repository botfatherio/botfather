#ifndef SCRIPTREPOSITORY_H
#define SCRIPTREPOSITORY_H

#include <QString>
#include <QMetaType>
#include <QDataStream>

class ScriptRepository
{
public:
	ScriptRepository();
	ScriptRepository(const QString name, const QString developer, const QString description, const QString repository);

	bool isValid() const;
	bool isLocal() const;
	bool isRemote() const;

	QString name() const;
	void setName(const QString &name);

	QString developer() const;
	void setDeveloper(const QString &developer);

	QString description() const;
	void setDescription(const QString &description);

	QString repository() const;
	void setRepository(const QString &repository);

private:
	QString m_name;
	QString m_developer;
	QString m_description;
	QString m_repository;
};

Q_DECLARE_METATYPE(ScriptRepository)

inline QDataStream &operator<<(QDataStream &stream, const ScriptRepository &script)
{
	return stream << script.name() << script.developer() << script.description() << script.repository();
}

inline QDataStream &operator>>(QDataStream &stream, ScriptRepository &script)
{
	QString name, developer, description, repository;
	stream >> name >> developer >> description >> repository;
	script.setName(name);
	script.setDeveloper(developer);
	script.setDescription(description);
	script.setRepository(repository);
	return stream;
}

#endif // SCRIPTREPOSITORY_H
