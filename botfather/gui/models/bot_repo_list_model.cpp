#include "bot_repo_list_model.hpp"
#include <QDataStream>
#include <QFile>
#include <QDebug>

BotRepoListModel::BotRepoListModel(QObject *parent) : QAbstractListModel(parent)
{
	qRegisterMetaType<QVector<BotRepo>>("QVector<BotRepo>");
}

int BotRepoListModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return m_bot_repos.size();
}

int BotRepoListModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 4;
}

QVariant BotRepoListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || index.row() > rowCount() || index.row() < 0)
	{
		return QVariant();
	}

	BotRepo bot_repo = m_bot_repos.at(index.row());

	if (role == NativeDataRole)
	{
		return QVariant::fromValue(bot_repo);
	}

	if (role == KeywordsRole)
	{
		QStringList keyword_parts;
		keyword_parts << bot_repo.name() << bot_repo.developer() << bot_repo.description();
		return keyword_parts.join(" ");
	}

	if (role == Qt::DisplayRole)
	{
		switch (index.column())
		{
		case 0: return bot_repo.name();
		case 1: return bot_repo.platform();
		case 2: return bot_repo.developer();
		case 3: return bot_repo.description();
		case 4: return bot_repo.gitUrl();
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
	case 1: return "Platform";
	case 2: return "Developer";
	case 3: return "Description";
	case 4: return "Url";
	default: return QVariant();
	}
}

bool BotRepoListModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	beginInsertRows(parent, position, position + rows - 1);

	for (int row = 0; row < rows; ++row)
	{
		m_bot_repos.insert(position, BotRepo());
	}

	endInsertRows();
	return true;
}

bool BotRepoListModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(parent, position, position + rows - 1);

	for (int row = 0; row < rows; ++row)
	{
		m_bot_repos.removeAt(position);
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
		BotRepo bot_repo = qvariant_cast<BotRepo>(value);
		m_bot_repos.replace(index.row(), bot_repo);

		emit(dataChanged(index, index));
		return true;
	}

	return false;
}

static bool remoteScriptNameLess(const BotRepo &br1, const BotRepo &br2)
{
	return br1.name() < br2.name();
}

static bool remoteScriptDevlLess(const BotRepo &br1, const BotRepo &br2)
{
	return br1.developer() < br2.developer();
}

static bool remoteScriptDescLess(const BotRepo &br1, const BotRepo &br2)
{
	return br1.description() < br2.description();
}

void BotRepoListModel::sort(int column, Qt::SortOrder order)
{
	std::function<bool(const BotRepo &, const BotRepo &)> comparator_function;

	if (column == 0) comparator_function = remoteScriptNameLess;
	if (column == 1) comparator_function = remoteScriptDevlLess;
	if (column == 2) comparator_function = remoteScriptDescLess;

	std::sort(m_bot_repos.begin(), m_bot_repos.end(), comparator_function);

	if (order == Qt::AscendingOrder)
	{
		std::reverse(m_bot_repos.begin(), m_bot_repos.end());
	}

	emit dataChanged(QModelIndex(), QModelIndex());
}

void BotRepoListModel::addBotRepos(const QVector<BotRepo> &bot_repo_list)
{
	for (BotRepo bot_repo : bot_repo_list)
	{
		insertRows(rowCount(), 1, QModelIndex());
		QModelIndex row_index = index(rowCount() - 1, 0, QModelIndex());
		setData(row_index, QVariant::fromValue(bot_repo), NativeDataRole);
	}
}
