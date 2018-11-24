#include "scriptreposmodel.h"
#include <QDataStream>
#include <QFile>
#include <QDebug>

ScriptReposModel::ScriptReposModel(QObject *parent) : QAbstractListModel(parent)
{

}

int ScriptReposModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return repositories.size();
}

int ScriptReposModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 6;
}

QVariant ScriptReposModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || index.row() > rowCount() || index.row() < 0)
	{
		return QVariant();
	}

	ScriptRepository *script = repositories.at(index.row());

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
			case ScriptRepository::Status::Outdated: return "Outdated";
			case ScriptRepository::Status::UpToDate: return "Up to date";
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

QVariant ScriptReposModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool ScriptReposModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	beginInsertRows(parent, position, position + rows - 1);

	for (int row = 0; row < rows; ++row)
	{
		repositories.insert(position, new ScriptRepository());
	}

	endInsertRows();
	return true;
}

bool ScriptReposModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(parent, position, position + rows - 1);

	for (int row = 0; row < rows; ++row)
	{
		repositories.removeAt(position);
	}

	endRemoveRows();
	return true;
}

bool ScriptReposModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid())
	{
		return false;
	}

	if (role == NativeDataRole)
	{
		ScriptRepository *script = qvariant_cast<ScriptRepository*>(value);
		repositories.replace(index.row(), script);

		emit(dataChanged(index, index));
		return true;
	}

	return false;
}

static bool remoteScriptNameLess(const ScriptRepository *rs1, const ScriptRepository *rs2)
{
	return rs1->name() < rs2->name();
}

static bool remoteScriptDevlLess(const ScriptRepository *rs1, const ScriptRepository *rs2)
{
	return rs1->developer() < rs2->developer();
}

static bool remoteScriptDescLess(const ScriptRepository *rs1, const ScriptRepository *rs2)
{
	return rs1->description() < rs2->description();
}

void ScriptReposModel::sort(int column, Qt::SortOrder order)
{
	std::function<bool(const ScriptRepository *, const ScriptRepository *)> comparator_function;

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


void ScriptReposModel::load(const QString &filename, bool filter_invalid)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	QVector<ScriptRepository::Data> repo_data;
	QDataStream in(&file);
	in >> repo_data;

	for (ScriptRepository::Data repo_date : repo_data)
	{
		ScriptRepository *repository = new ScriptRepository(repo_date);
		if (!filter_invalid || repository->isValid())
		{
			addEntry(repository);
		}
	}

	file.close();
}

void ScriptReposModel::save(const QString &filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly))
	{
		return;
	}

	// Data has to be stored the same way it's loaded, meaning we have to have it in an QVector.
	QVector<ScriptRepository::Data> data_list;
	for (ScriptRepository *repo : repositories)
	{
		data_list << repo->data();
	}

	QDataStream out(&file);
	out << data_list;
	file.close();
}

void ScriptReposModel::addEntry(ScriptRepository *script)
{
	insertRows(rowCount(), 1, QModelIndex());
	QModelIndex row_index = index(rowCount() - 1, 0, QModelIndex());
	setData(row_index, QVariant::fromValue(script), NativeDataRole);

	// Only the status of local scripts can be checked
	if (!script->localPath().isEmpty())
	{
		script->checkStatus();
	}
}
