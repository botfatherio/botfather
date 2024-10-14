#ifndef BOTLISTMODEL_H
#define BOTLISTMODEL_H

#include <QAbstractListModel>
#include <QVector>

#include "bot.hpp"

class BotListModel : public QAbstractListModel {
    Q_OBJECT

   public:
    explicit BotListModel(QObject *parent = nullptr);

    static const int BOT_PTR_ROLE = Qt::UserRole + 1;
    static const int BOT_PATH_ROLE = Qt::UserRole + 2;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    bool insertRows(int row, int count,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count,
                    const QModelIndex &parent = QModelIndex()) override;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
    void save(const QString &filepath);
    void load(const QString &filepath);
    void loadLegacy(const QString &filepath);
    void list(const QString &path, const QString &name, const QString &repo,
              const QString &branch);

    QString defaultLocation() const;
    QString legacyLocation() const;
    int numberOfRunningBots() const;

   signals:
    void botListed(Bot *bot);
    void botRemoved(const QString &bot_path);

   private:
    QVector<Bot *> m_bots;
};

#endif  // BOTLISTMODEL_H
