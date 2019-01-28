#ifndef SCRIPTREPOSITORY_H
#define SCRIPTREPOSITORY_H

#include <QObject>
#include <QString>
#include <QMetaType>
#include <QDataStream>

class BotRepo : public QObject
{
	Q_OBJECT

public:
	struct Data
	{
		QString name;
		QString developer;
		QString description;
		QString local_path;
		QString remote_url;

		Data(){}
		Data(const QString &name, const QString &developer, const QString &description, const QString &local_path, const QString &remote_url)
		{
			this->name = name;
			this->developer = developer;
			this->description = description;
			this->local_path = local_path;
			this->remote_url = remote_url;
		}

		bool isEmpty() const
		{
			return local_path.isEmpty() && remote_url.isEmpty();
		}
	};

	explicit BotRepo(QObject *parent = nullptr);
	explicit BotRepo(BotRepo::Data data, QObject *parent = nullptr);


	bool isValid() const;
	QString findScriptPath() const;

	Data data() const;

	QString name() const;
	void setName(const QString &name);

	QString developer() const;
	void setDeveloper(const QString &developer);

	QString description() const;
	void setDescription(const QString &description);

	QString localPath() const;
	void setLocalPath(const QString &path);

	QString remoteUrl() const;
	void setRemoteUrl(const QString &url);

private:
	Data m_data;
};

Q_DECLARE_METATYPE(BotRepo*)

inline QDataStream &operator<<(QDataStream &stream, const BotRepo::Data &data)
{
	return stream << data.name << data.developer << data.description << data.local_path << data.remote_url;
}

inline QDataStream &operator>>(QDataStream &stream, BotRepo::Data &data)
{
	return stream >> data.name >> data.developer >> data.description >> data.local_path >> data.remote_url;
}

#endif // SCRIPTREPOSITORY_H
