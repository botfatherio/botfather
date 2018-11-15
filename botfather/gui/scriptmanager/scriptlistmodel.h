#ifndef SCRIPTLISTMODEL_H
#define SCRIPTLISTMODEL_H

#include <QAbstractListModel>
#include "scriptrepository.h"

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

	ScriptRepository remoteScript(const QModelIndex &index) const;
	void addRemoteScript(const ScriptRepository &script);

	void load(const QString &filename);
	void save(const QString &filename);

private:
	QVector<ScriptRepository> remote_scripts;
};

#endif // SCRIPTLISTMODEL_H
