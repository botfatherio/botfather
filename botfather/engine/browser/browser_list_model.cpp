#include "browser_list_model.hpp"

#include <QDebug>

#include "browser.hpp"
#include "browser_creator.hpp"
#include "browser_util.hpp"
#include "include/wrapper/cef_helpers.h"

BrowserListModel::BrowserListModel(QObject *parent)
    : QAbstractTableModel(parent) {}

int BrowserListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_browsers.size();
}

int BrowserListModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 3;  // Group Name, Browser ID, Current URL
}

QVariant BrowserListModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return QVariant();
    }

    switch (section) {
        case 0:
            return "Group";
        case 1:
            return "ID";
        case 2:
            return "URL";
        default:
            return QVariant();
    }
}

QVariant BrowserListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    Browser *browser = m_browsers.at(index.row());

    if (!browser) {
        return QVariant();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
            case 0:
                return browser->group();
            case 1:
                return browser->name();
            case 2:
                return browser->url();
        }
    }

    if (role == BROWSER_PTR_ROLE) {
        return QVariant::fromValue(browser);
    }

    if (role == BROWSER_UID_ROLE) {
        return createUID(browser->group(), browser->name());
    }

    return QVariant();
}

bool BrowserListModel::setData(const QModelIndex &index, const QVariant &value,
                               int role) {
    if (!index.isValid() || role != BROWSER_PTR_ROLE) {
        return false;
    }

    Browser *browser = qvariant_cast<Browser *>(value);
    m_browsers.replace(index.row(), browser);

    emit dataChanged(index, index);
    return true;
}

bool BrowserListModel::insertRows(int position, int count,
                                  const QModelIndex &parent) {
    beginInsertRows(parent, position, position + count - 1);

    for (int row = 0; row < count; ++row) {
        m_browsers.insert(position, nullptr);
    }

    endInsertRows();
    return true;
}

bool BrowserListModel::removeRows(int position, int count,
                                  const QModelIndex &parent) {
    beginRemoveRows(parent, position, position + count - 1);

    for (int row = 0; row < count; ++row) {
        m_browsers.removeAt(position);
    }

    endRemoveRows();
    return true;
}

void BrowserListModel::addBrowser(Browser *browser) {
    insertRows(rowCount(), 1, QModelIndex());
    QModelIndex row_index = index(rowCount() - 1, 0, QModelIndex());
    setData(row_index, QVariant::fromValue(browser), BROWSER_PTR_ROLE);
    qDebug() << "Adding a browser to model:" << browser->group()
             << browser->name();
}

void BrowserListModel::removeBrowser(Browser *browser) {
    QModelIndex match = findBrowser(browser);
    if (!match.isValid()) return;
    removeRow(match.row());
}

QList<Browser *> BrowserListModel::browsers() const { return m_browsers; }

QModelIndex BrowserListModel::findBrowser(Browser *browser) {
    return match(index(0, 0), BROWSER_UID_ROLE,
                 createUID(browser->group(), browser->name()), 1,
                 Qt::MatchExactly)
        .first();
}

QString BrowserListModel::createUID(const QString &group,
                                    const QString &browser_id) {
    // Using a seperator sequence that will unlikely occure in a Bots path.
    // (Since the bots path is used as group id of the Browser and we want
    // to prevent naming collisions).
    return group + "{////}" + browser_id;
}
