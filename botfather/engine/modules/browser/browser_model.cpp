#include "browser_model.h"
#include <QDebug>
#include "include/wrapper/cef_helpers.h"
#include "browser_creator.h"
#include "browser_util.h"
#include "../../types/browser.h"

BrowserModel::BrowserModel(QObject *parent)
	: QAbstractTableModel(parent)
{

}

int BrowserModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return m_browsers.size();
}

int BrowserModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 3; // Group Name, Browser ID, Current URL
}

QVariant BrowserModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
	{
		return QVariant();
	}

	switch (section) {
	case 0: return "Group";
	case 1: return "ID";
	case 2: return "URL";
	default: return QVariant();
	}
}

QVariant BrowserModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	Browser* browser = m_browsers.at(index.row());

	if (!browser)
	{
		return QVariant();
	}

	if (role == Qt::DisplayRole || role == Qt::EditRole)
	{
		switch (index.column())
		{
		case 0: return browser->group();
		case 1: return browser->name();
		case 2: return browser->url();
		}
	}

	if (role == BROWSER_PTR_ROLE)
	{
		return QVariant::fromValue(browser);
	}

	if (role == BROWSER_UID_ROLE)
	{
		return createUID(browser->group(), browser->name());
	}

	return QVariant();
}

bool BrowserModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid() || role != BROWSER_PTR_ROLE)
	{
		return false;
	}

	Browser *browser = qvariant_cast<Browser*>(value);
	m_browsers.replace(index.row(), browser);
	// FIXME: make sure the replaced browser gets deleted

	emit dataChanged(index, index);
	return true;
}

bool BrowserModel::insertRows(int position, int count, const QModelIndex &parent)
{
	beginInsertRows(parent, position, position + count - 1);

	for (int row = 0; row < count; ++row)
	{
		m_browsers.insert(position, nullptr);
	}

	endInsertRows();
	return true;
}

bool BrowserModel::removeRows(int position, int count, const QModelIndex &parent)
{
	beginRemoveRows(parent, position, position + count - 1);

	for (int row = 0; row < count; ++row)
	{
		Browser *browser = m_browsers.takeAt(position);
		if (!browser) continue;

		// FIXME: handle deletion / check this approche
		BrowserManager::closeCefBrowser(browser->cefBrowser());
		delete browser;
	}

	endRemoveRows();
	return true;
}

void BrowserModel::addBrowser(Browser *browser)
{
	insertRows(rowCount(), 1, QModelIndex());
	QModelIndex row_index = index(rowCount() - 1, 0, QModelIndex());
	setData(row_index, QVariant::fromValue(browser), BROWSER_PTR_ROLE);
	qDebug() << "Adding a browser to model:" << browser->group() << browser->name();
}

QList<Browser*> BrowserModel::browsers() const
{
	return m_browsers;
}

QString BrowserModel::createUID(const QString &group, const QString &browser_id)
{
	return group + "--" + browser_id;
}
