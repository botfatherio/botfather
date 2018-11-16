#include "scriptlistmodel.h"
#include <QDataStream>
#include <QFile>
#include <QDebug>

ScriptListModel::ScriptListModel(QObject *parent) : QAbstractListModel(parent)
{
	// TODO: Use | seperated options to decide which data to display
}

int ScriptListModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return remote_scripts.size();
}

int ScriptListModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 3;
}

QVariant ScriptListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || index.row() > rowCount() || index.row() < 0)
	{
		return QVariant();
	}

	ScriptRepository script = remote_scripts.at(index.row());

	if (role == NativeDataRole)
	{
		return QVariant::fromValue(script);
	}

	if (role == KeywordsRole)
	{
		QStringList keyword_parts;
		keyword_parts << script.name() << script.developer() << script.description();
		return keyword_parts.join(" ");
	}

	if (role == Qt::DisplayRole)
	{
		switch (index.column())
		{
		case 0: return script.name();
		case 1: return script.developer();
		case 2: return script.description();
		}
	}

	return QVariant();
}

QVariant ScriptListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
	{
		return QVariant();
	}

	switch (section) {
	case 0: return "Name";
	case 1: return "Developer";
	case 2: return "Description";
	default: return QVariant();
	}
}

bool ScriptListModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	beginInsertRows(parent, position, position + rows - 1);

	for (int row = 0; row < rows; ++row)
	{
		remote_scripts.insert(position, ScriptRepository());
	}

	endInsertRows();
	return true;
}

bool ScriptListModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(parent, position, position + rows - 1);

	for (int row = 0; row < rows; ++row)
	{
		remote_scripts.removeAt(position);
	}

	endRemoveRows();
	return true;
}

bool ScriptListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid())
	{
		return false;
	}

	if (role == NativeDataRole)
	{
		ScriptRepository script = qvariant_cast<ScriptRepository>(value);
		remote_scripts.replace(index.row(), script);

		emit(dataChanged(index, index));
		return true;
	}

	return false;
}

static bool remoteScriptNameLess(const ScriptRepository& rs1, const ScriptRepository& rs2)
{
	return rs1.name() < rs2.name();
}

static bool remoteScriptDevlLess(const ScriptRepository& rs1, const ScriptRepository& rs2)
{
	return rs1.developer() < rs2.developer();
}

static bool remoteScriptDescLess(const ScriptRepository& rs1, const ScriptRepository& rs2)
{
	return rs1.description() < rs2.description();
}

void ScriptListModel::sort(int column, Qt::SortOrder order)
{
	std::function<bool(const ScriptRepository &, const ScriptRepository &)> comparator_function;

	if (column == 0) comparator_function = remoteScriptNameLess;
	if (column == 1) comparator_function = remoteScriptDevlLess;
	if (column == 2) comparator_function = remoteScriptDescLess;

	std::sort(remote_scripts.begin(), remote_scripts.end(), comparator_function);

	if (order == Qt::AscendingOrder)
	{
		std::reverse(remote_scripts.begin(), remote_scripts.end());
	}

	emit dataChanged(QModelIndex(), QModelIndex());
}

void ScriptListModel::load(const QString &filename)
{
	QFile file(filename);

	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	QDataStream in(&file);
	in >> remote_scripts;

	emit dataChanged(QModelIndex(), QModelIndex());
}

void ScriptListModel::save(const QString &filename)
{
	QFile file(filename);

	if (!file.open(QIODevice::WriteOnly))
	{
		return;
	}

	QDataStream out(&file);
	out << remote_scripts;
}

void ScriptListModel::addEntry(const ScriptRepository &script)
{
	insertRows(rowCount(), 1, QModelIndex());
	QModelIndex row_index = index(rowCount() -1, 0, QModelIndex());
	setData(row_index, QVariant::fromValue(script), NativeDataRole);
}
