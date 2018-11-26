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

	explicit ScriptRepository(QObject *parent = nullptr);
	explicit ScriptRepository(ScriptRepository::Data data, QObject *parent = nullptr);

	enum class Status
	{
		Unavailabe,
		UpToDate,
		Outdated,
	};

	bool isValid() const;
	QString findScriptPath() const;

	Status status() const;
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

public slots:
	void checkStatus();
	void noteDifferencesToRemote(int differences_to_remote);

signals:
	void statusChanged(const Status &status);

private:
	Data m_data;
	Status m_status = Status::Unavailabe;
};

Q_DECLARE_METATYPE(ScriptRepository*)

inline QDataStream &operator<<(QDataStream &stream, const ScriptRepository::Data &data)
{
	return stream << data.name << data.developer << data.description << data.local_path << data.remote_url;
}

inline QDataStream &operator>>(QDataStream &stream, ScriptRepository::Data &data)
{
	return stream >> data.name >> data.developer >> data.description >> data.local_path >> data.remote_url;
}

#endif // SCRIPTREPOSITORY_H
