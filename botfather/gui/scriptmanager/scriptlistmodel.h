#ifndef SCRIPTLISTMODEL_H
#define SCRIPTLISTMODEL_H

#include <QAbstractListModel>

struct RemoteScript // TODO: rename and move to seperate file
{
	QString display_name;
	QString developer;
	QString description;
	QString repository;

	RemoteScript() {}
	RemoteScript(QString dn, QString dv, QString rp, QString ds)
	{
		display_name = dn;
		developer = dv;
		description = ds;
		repository = rp;
	}
};

Q_DECLARE_METATYPE(RemoteScript)

inline QDataStream &operator<<(QDataStream &stream, const RemoteScript &script)
{
	return stream << script.display_name << script.developer << script.description << script.repository;
}

inline QDataStream &operator>>(QDataStream &stream, RemoteScript &script)
{
	return stream >> script.display_name >> script.developer >> script.description >> script.repository;
}

class ScriptListModel : public QAbstractListModel
{
	Q_OBJECT

public:
	ScriptListModel(QObject *parent = nullptr);

	static const int NativeDataRole = Qt::UserRole + 1;
	static const int KeywordsRole = Qt::UserRole + 2;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent) const override;

	QVariant data(const QModelIndex &index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	bool insertRows(int position, int rows, const QModelIndex &parent) override;
	bool removeRows(int position, int rows, const QModelIndex &parent) override;

	/*bool setData(const QModelIndex &index, const QVariant &value, int role) override;*/
	void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

	RemoteScript remoteScript(const QModelIndex &index) const;
	void addRemoteScript(const RemoteScript &script);

	void load(const QString &filename);
	void save(const QString &filename);

private:
	QVector<RemoteScript> remote_scripts;
};

#endif // SCRIPTLISTMODEL_H
