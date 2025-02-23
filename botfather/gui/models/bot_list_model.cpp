#include "bot_list_model.hpp"

#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>

BotListModel::BotListModel(QObject *parent) : QAbstractListModel(parent) {
    // Required by the QAbstractItemModel::dataChanged signal
    qRegisterMetaType<QVector<int>>("QVector<int>");
}

int BotListModel::rowCount(const QModelIndex &parent) const {
    // For list models only the root node (an invalid parent) should return the
    // list's size. For all other (valid) parents, rowCount() should return 0 so
    // that it does not become a tree model.
    if (parent.isValid()) {
        return 0;
    }

    return m_bots.size();
}

int BotListModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    }

    return 3;  // Name, path, repo
}

QVariant BotListModel::headerData(int section, Qt::Orientation orientation,
                                  int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return QVariant();
    }

    switch (section) {
        case 0:
            return "Bots";  // Showing "Bots" looks fancier than showing "Name"
                            // as header which would be more accurate
        case 1:
            return "Path";
        case 2:
            return "Repo";
        default:
            return QVariant();
    }
}

Qt::ItemFlags BotListModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    if (index.column() == 0) {
        // Only a bots name is editable
        return Qt::ItemIsEditable | QAbstractListModel::flags(index);
    }

    return QAbstractListModel::flags(index);
}

QVariant BotListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    Bot *bot = m_bots.at(index.row());

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
            case 0:
                return bot->name();
            case 1:
                return bot->path();
            case 2:
                return bot->repo();
        }
    }

    if (role == Qt::DecorationRole && index.column() == 0) {
        if (bot->isRunning())
            return QIcon(":/icons/icons/icons8-running-rabbit-32.png");
        else
            return QIcon(":/icons/icons/icons8-sleep-32.png");
    }

    if (role == BOT_PTR_ROLE) {
        return QVariant::fromValue(bot);
    }

    if (role == BOT_PATH_ROLE) {
        return bot->path();
    }

    return QVariant();
}

bool BotListModel::setData(const QModelIndex &index, const QVariant &value,
                           int role) {
    if (!index.isValid() || data(index, role) == value) {
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

bool BotListModel::insertRows(int position, int count,
                              const QModelIndex &parent) {
    beginInsertRows(parent, position, position + count - 1);

    for (int row = 0; row < count; ++row) {
        m_bots.insert(position, new Bot(this));
    }

    endInsertRows();
    return true;
}

bool BotListModel::removeRows(int position, int count,
                              const QModelIndex &parent) {
    beginRemoveRows(parent, position, position + count - 1);

    for (int row = 0; row < count; ++row) {
        Bot *bot = m_bots.takeAt(position);
        emit botRemoved(bot->path());
        bot->deleteLater();
    }

    endRemoveRows();
    return true;
}

static bool botNameLess(const Bot *bot1, const Bot *bot2) {
    return bot1->name() < bot2->name();
}

void BotListModel::sort(int /*column*/, Qt::SortOrder order) {
    std::sort(m_bots.begin(), m_bots.end(), botNameLess);

    if (order == Qt::AscendingOrder) {
        std::reverse(m_bots.begin(), m_bots.end());
    }

    // This signals that all items changed (views will thus be updated).
    emit dataChanged(QModelIndex(), QModelIndex());
}

void BotListModel::save(const QString &filepath) {
    QFileInfo info(filepath);
    if (!QDir().mkpath(info.absolutePath())) {
        qDebug() << "Unable to create the dir path for" << filepath;
        return;
    }

    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open" << filepath
                 << "to store the bot list data in it";
        return;
    }

    QJsonArray json_bots_array;
    for (Bot *bot : m_bots) {
        QJsonObject json_bot_object;
        json_bot_object["name"] = bot->name();
        json_bot_object["path"] = bot->path();
        json_bot_object["repo"] = bot->repo();
        json_bot_object["branch"] = bot->branch();
        json_bots_array.append(json_bot_object);
    }

    QJsonDocument json_doc(json_bots_array);
    file.write(json_doc.toJson());
    file.close();
}

void BotListModel::load(const QString &filepath) {
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open" << filepath
                 << "to load BotListModel data from";
        return;
    }

    QJsonDocument json_doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!json_doc.isArray()) {
        qDebug() << "The bot list file" << filepath
                 << "does not contain a JSON array";
        return;
    }

    QJsonArray json_bots_array = json_doc.array();
    for (const QJsonValue &json_bot_value : json_bots_array) {
        QJsonObject json_bot_object = json_bot_value.toObject();
        list(json_bot_object["path"].toString(),
             json_bot_object["name"].toString(),
             json_bot_object["repo"].toString(),
             json_bot_object["branch"].toString());
    }
}

void BotListModel::loadLegacy(const QString &filepath) {
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open" << filepath
                 << "to load legacy bot list data from";
        return;
    }

    QVector<Bot::LegacyData> bot_data_list;
    QDataStream in(&file);
    in >> bot_data_list;

    for (Bot::LegacyData bot_data : bot_data_list) {
        qDebug() << "Listing bot" << bot_data.name << bot_data.path;
        list(bot_data.path, bot_data.name, bot_data.repo, QString());
    }

    file.close();
}

void BotListModel::list(const QString &path, const QString &name,
                        const QString &repo, const QString &branch) {
    Bot *temp_bot = new Bot(path, name, repo, branch);
    bool is_valid = temp_bot->isValid();
    delete temp_bot;

    if (!is_valid) {
        qDebug() << "The bot (" << path
                 << ") is invalid and will not be listed";
        return;
    }

    if (!insertRows(rowCount(), 1)) {
        qDebug() << "Failed to insert a new row into the BotListModel";
        return;
    }

    setData(index(rowCount() - 1, 0), name, Qt::DisplayRole);
    setData(index(rowCount() - 1, 1), path, Qt::DisplayRole);
    setData(index(rowCount() - 1, 2), repo, Qt::DisplayRole);
    setData(index(rowCount() - 1, 3), branch, Qt::DisplayRole);

    Bot *bot_to_list =
        qvariant_cast<Bot *>(data(index(rowCount() - 1), BOT_PTR_ROLE));
    emit botListed(bot_to_list);
}

QString BotListModel::defaultLocation() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) +
           "/bots.json";
}

QString BotListModel::legacyLocation() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) +
           "/bots.dat";
}

int BotListModel::numberOfRunningBots() const {
    int number_of_running_bots = 0;
    for (Bot *bot : m_bots) {
        if (bot->isRunning()) {
            number_of_running_bots++;
        }
    }
    return number_of_running_bots;
}
