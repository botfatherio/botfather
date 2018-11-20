#ifndef SCRIPTREPOSITORY_H
#define SCRIPTREPOSITORY_H

#include <QObject>
#include <QString>
#include <QMetaType>
#include <QDataStream>

class ScriptRepository : public QObject
{
	Q_OBJECT

public:
	struct Data
	{
		QString name;
		QString developer;
		QString description;
		QString repository;

		Data(){}
		Data(const QString &name, const QString &developer, const QString &description, const QString &repository)
		{
			this->name = name;
			this->developer = developer;
			this->description = description;
			this->repository = repository;
		}
	};

	explicit ScriptRepository(QObject *parent = nullptr);
	explicit ScriptRepository(ScriptRepository::Data data, QObject *parent = nullptr);
	explicit ScriptRepository(const QString name, const QString developer, const QString description, const QString repository, QObject *parent = nullptr);

	enum class Status
	{
		Unavailabe,
		UpToDate,
		Outdated,
	};

	bool isValid() const;
	bool isLocal() const;
	bool isRemote() const;

	Status status() const;
	Data data() const;

	QString name() const;
	void setName(const QString &name);

	QString developer() const;
	void setDeveloper(const QString &developer);

	QString description() const;
	void setDescription(const QString &description);

	QString repository() const;
	void setRepository(const QString &repository);

private:
	Data m_data;
};

Q_DECLARE_METATYPE(ScriptRepository*)

inline QDataStream &operator<<(QDataStream &stream, const ScriptRepository::Data &data)
{
	return stream << data.name << data.developer << data.description << data.repository;
}

inline QDataStream &operator>>(QDataStream &stream, ScriptRepository::Data &data)
{
	return stream >> data.name >> data.developer >> data.description >> data.repository;
}

#endif // SCRIPTREPOSITORY_H
