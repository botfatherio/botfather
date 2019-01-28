#ifndef SCRIPTREPOSMODEL_H
#define SCRIPTREPOSMODEL_H

#include <QAbstractListModel>
#include "botrepo.h"

class BotRepoListModel : public QAbstractListModel
{
	Q_OBJECT

public:
	BotRepoListModel(QObject *parent = nullptr);

	static const int NativeDataRole = Qt::UserRole + 1;
	static const int KeywordsRole = Qt::UserRole + 2;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;
	bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
	void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

public slots:
	void load(const QString &filename, bool filter_invalid = true);
	void save(const QString &filename);
	void addEntry(BotRepo *repository);

private:
	QVector<BotRepo*> repositories;
};

#endif // SCRIPTREPOSMODEL_H
