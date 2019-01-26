#include "botlistmodel.h"
#include <QFile>
#include <QIcon>
#include <QDataStream>
#include <QStandardPaths>
#include <QDebug>

BotListModel::BotListModel(QObject *parent)
	: QAbstractListModel(parent)
{

}

int BotListModel::rowCount(const QModelIndex &parent) const
{
	// For list models only the root node (an invalid parent) should return the list's size. For all
	// other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
	if (parent.isValid())
	{
		return 0;
	}

	return m_bots.size();
}

int BotListModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
	{
		return 0;
	}

	return 3; // Name, path, repo
}

QVariant BotListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
	{
		return QVariant();
	}

	switch (section) {
	case 0: return "Bots"; // Showing "Bots" looks fancier than showing "Name" as header which would be more accurate
	case 1: return "Path";
	case 2: return "Repo";
	default: return QVariant();
	}
}

Qt::ItemFlags BotListModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return Qt::NoItemFlags;
	}

	if (index.column() == 0)
	{
		// Only a bots name is editable
		return Qt::ItemIsEditable | QAbstractListModel::flags(index);
	}

	return QAbstractListModel::flags(index);
}

QVariant BotListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	Bot *bot = m_bots.at(index.row());

	if (role == Qt::DisplayRole || role == Qt::EditRole)
	{
		switch (index.column())
		{
		case 0: return bot->name();
		case 1: return bot->path();
		case 2: return bot->repo();
		}
	}

	if (role == Qt::DecorationRole && index.column() == 0)
	{
		if (bot->isRunning()) return QIcon(":/icons/icons/icons8-running-rabbit-64.png");
		else return QIcon(":/icons/icons/icons8-sleep-64.png");
	}

	return QVariant();
}

bool BotListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid() || data(index, role) == value)
	{
		return false;
	}

	Bot *bot = m_bots.at(index.row());

	switch (index.column()) {
	case 0:
		if (!value.toString().isEmpty()) bot->setName(value.toString());
		break;
	case 1:
		if (!value.toString().isEmpty()) bot->setPath(value.toString());
		break;
	case 2:
		if (!value.toString().isEmpty()) bot->setRepo(value.toString());
		break;
	default:
		return false;
	}

	emit dataChanged(index, index, QVector<int>() << role);
	return true;
}

bool BotListModel::insertRows(int position, int count, const QModelIndex &parent)
{
	beginInsertRows(parent, position, position + count - 1);

	for (int row = 0; row < count; ++row)
	{
		m_bots.insert(position, new Bot(this));
	}

	endInsertRows();
	return true;
}

bool BotListModel::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(parent, row, row + count - 1);

	for (int row = 0; row < count; ++row)
	{
		m_bots.removeAt(row);
	}

	endRemoveRows();
	return true;
}

void BotListModel::save(const QString &filepath)
{
	QFile file(filepath);
	if (!file.open(QIODevice::WriteOnly))
	{
		qDebug() << "Failed to open" << filepath << "to store the BotListModel data in it";
		return;
	}

	// Data must be stored using the same structur it's loaded from (otherwise loading will fail).
	QVector<Bot::Data> bot_data_list;
	for (Bot *bot : m_bots)
	{
		bot_data_list << bot->data();
	}

	QDataStream out(&file);
	out << bot_data_list;
	file.close();
}

void BotListModel::load(const QString &filepath)
{
	QFile file(filepath);
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "Failed to open" << filepath << "to load BotListModel data from";
		return;
	}

	QVector<Bot::Data> bot_data_list;
	QDataStream in(&file);
	in >> bot_data_list;

	for (Bot::Data bot_data : bot_data_list)
	{
		qDebug() << "Listing bot" << bot_data.path;
		list(bot_data);
	}

	file.close();
}

void BotListModel::list(const Bot::Data &bot_data)
{
	Bot *bot = new Bot(bot_data);
	bool is_valid = bot->isValid();
	delete bot;

	if (!is_valid)
	{
		qDebug() << "The bot (" << bot_data.path << ") is invalid and will not be listed";
		return;
	}

	if (!insertRows(rowCount(), 1))
	{
		qDebug() << "Failed to insert a new row into the BotListModel";
		return;
	}

	setData(index(rowCount() - 1, 0), bot_data.name, Qt::DisplayRole);
	setData(index(rowCount() - 1, 1), bot_data.path, Qt::DisplayRole);
	setData(index(rowCount() - 1, 2), bot_data.repo, Qt::DisplayRole);
}

QString BotListModel::defaultLocation() const
{
	// TODO: Think about whether this method belongs here or not.
	return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/bots.dat";
}
