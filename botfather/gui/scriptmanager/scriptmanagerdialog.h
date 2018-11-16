#ifndef SCRIPTBROWSERDIALOG_H
#define SCRIPTBROWSERDIALOG_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include "scriptlistmodel.h"

namespace Ui {
class ScriptManagerDialog;
}

class ScriptManagerDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ScriptManagerDialog(QWidget *parent = nullptr);
	~ScriptManagerDialog();

public slots:
	void searchOnline(const QString &query);
	void searchLocal(const QString &query);
	void itemDoubleClicked(const QModelIndex &index);

private:
	Ui::ScriptManagerDialog *ui;
	ScriptListModel *online_scripts_model;
	ScriptListModel *local_scripts_model;
	QSortFilterProxyModel *online_scripts_proxy;
	QSortFilterProxyModel *local_scripts_proxy;
};

#endif // SCRIPTBROWSERDIALOG_H
