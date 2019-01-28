#include "botrepolistmodel.h"
#include <QDataStream>
#include <QFile>
#include <QDebug>

BotRepoListModel::BotRepoListModel(QObject *parent) : QAbstractListModel(parent)
{

}

int BotRepoListModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return repositories.size();
}

int BotRepoListModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 6;
}

QVariant BotRepoListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || index.row() > rowCount() || index.row() < 0)
	{
		return QVariant();
	}

	BotRepo *script = repositories.at(index.row());

	if (role == NativeDataRole)
	{
		return QVariant::fromValue(script);
	}

	if (role == KeywordsRole)
	{
		QStringList keyword_parts;
		keyword_parts << script->name() << script->developer() << script->description();
		return keyword_parts.join(" ");
	}

	if (role == Qt::DisplayRole)
	{
		switch (index.column())
		{
		case 0: return script->name();
		case 1: {
			switch (script->status()) {
			case BotRepo::Status::Outdated: return "Outdated";
			case BotRepo::Status::UpToDate: return "Up to date";
			default: return "Unavailable";
			}
		}
		case 2: return script->developer();
		case 3: return script->description();
		case 4: return script->localPath();
		case 5: return script->remoteUrl();
		}
	}

	return QVariant();
}

QVariant BotRepoListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
	{
		return QVariant();
	}

	switch (section) {
	case 0: return "Name";
	case 1: return "Status";
	case 2: return "Developer";
	case 3: return "Description";
	case 4: return "Local path";
	case 5: return "Remote url";
	default: return QVariant();
	}
}

bool BotRepoListModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	beginInsertRows(parent, position, position + rows - 1);

	for (int row = 0; row < rows; ++row)
	{
		repositories.insert(position, new BotRepo(this));
	}

	endInsertRows();
	return true;
}

bool BotRepoListModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(parent, position, position + rows - 1);

	for (int row = 0; row < rows; ++row)
	{
		repositories.removeAt(position);
	}

	endRemoveRows();
	return true;
}

bool BotRepoListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid())
	{
		return false;
	}

	if (role == NativeDataRole)
	{
		BotRepo *script = qvariant_cast<BotRepo*>(value);
		repositories.replace(index.row(), script);

		emit(dataChanged(index, index));
		return true;
	}

	return false;
}

static bool remoteScriptNameLess(const BotRepo *rs1, const BotRepo *rs2)
{
	return rs1->name() < rs2->name();
}

static bool remoteScriptDevlLess(const BotRepo *rs1, const BotRepo *rs2)
{
	return rs1->developer() < rs2->developer();
}

static bool remoteScriptDescLess(const BotRepo *rs1, const BotRepo *rs2)
{
	return rs1->description() < rs2->description();
}

void BotRepoListModel::sort(int column, Qt::SortOrder order)
{
	std::function<bool(const BotRepo *, const BotRepo *)> comparator_function;

	if (column == 0) comparator_function = remoteScriptNameLess;
	if (column == 1) comparator_function = remoteScriptDevlLess;
	if (column == 2) comparator_function = remoteScriptDescLess;

	std::sort(repositories.begin(), repositories.end(), comparator_function);

	if (order == Qt::AscendingOrder)
	{
		std::reverse(repositories.begin(), repositories.end());
	}

	emit dataChanged(QModelIndex(), QModelIndex());
}

void BotRepoListModel::load(const QString &filename, bool filter_invalid)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	QVector<BotRepo::Data> repo_data;
	QDataStream in(&file);
	in >> repo_data;

	for (BotRepo::Data repo_date : repo_data)
	{
		BotRepo *repository = new BotRepo(repo_date, this);
		if (!filter_invalid || repository->isValid())
		{
			addEntry(repository);
		}
	}

	file.close();
}

void BotRepoListModel::save(const QString &filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly))
	{
		return;
	}

	// Data has to be stored the same way it's loaded, meaning we have to have it in an QVector.
	QVector<BotRepo::Data> data_list;
	for (BotRepo *repo : repositories)
	{
		data_list << repo->data();
	}

	QDataStream out(&file);
	out << data_list;
	file.close();
}

void BotRepoListModel::addEntry(BotRepo *script)
{
	insertRows(rowCount(), 1, QModelIndex());
	QModelIndex row_index = index(rowCount() - 1, 0, QModelIndex());
	setData(row_index, QVariant::fromValue(script), NativeDataRole);
}

void BotRepoListModel::checkForUpdates()
{
	for (BotRepo *repo : repositories)
	{
		repo->checkStatus();
	}
}
