#ifndef BOT_H
#define BOT_H

#include <QObject>
#include <QThread>
#include "../../engine/engine.h"

class Bot : public QObject
{
	Q_OBJECT

public:
	// We use a struct to serialize a bots data because QObjects can't be serialized directly.
	struct Data
	{
		QString path; // Use the path to identify this bot
		QString name;
		QString repo; // BF ratings are bind to a repository, we don't want it to change.

		Data() {}
		Data(const QString &path, const QString &name, const QString &repo)
		{
			this->path = path;
			this->name = name;
			this->repo = repo;
		}
	};

	explicit Bot(QObject *parent = nullptr);
	explicit Bot(const Bot::Data &data, QObject *parent = nullptr);

	// Returns true if the bots name and path are empty.
	bool isNull() const;

	// Returns true if the bot isNull returns false and the bots path exists.
	bool isValid() const;

	Bot::Data data() const;
	void setData(const Bot::Data &data);

	// The path to the folder in which the bots files are stored. Use a bots path to identify it.
	QString path() const;
	void setPath(const QString &path);

	// The name given to the bot by the user.
	QString name() const;
	void setName(const QString &name);

	// The scripts remote git repository url. Returns an empty string if the bot is local (has no remote repo).
	QString repo() const;
	void setRepo(const QString &repo);

signals:

public slots:

private:
	Bot::Data m_data;

	//Engine *m_engine;
	//QThread *m_engine_thread;
};

Q_DECLARE_METATYPE(Bot*)

inline QDataStream &operator<<(QDataStream &stream, const Bot::Data &data)
{
	return stream << data.name << data.path;
}

inline QDataStream &operator>>(QDataStream &stream, Bot::Data &data)
{
	return stream >> data.name >> data.path;
}

#endif // BOT_H
