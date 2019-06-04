#ifndef BROWSERMODEL_H
#define BROWSERMODEL_H

#include <QAbstractTableModel>

class Browser;

class BrowserModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	explicit BrowserModel(QObject *parent = nullptr);
	static const int BROWSER_PTR_ROLE = Qt::UserRole + 1;
	static const int BROWSER_UID_ROLE = Qt::UserRole + 2;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

	bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

	void addBrowser(Browser *browser);
	QList<Browser*> browsers() const; // TODO: maybe remove this method, its just to quickyl implement the closeAllBrowsers method to prevent crashes

	static QString createUID(const QString &group, const QString &browser_id);

private:
	QList<Browser*> m_browsers;
};

#endif // BROWSERMODEL_H
