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
	void search();
	void itemDoubleClicked(const QModelIndex &index);

private:
	Ui::ScriptManagerDialog *ui;
	ScriptListModel *script_list_model;
	QSortFilterProxyModel *proxy_model;
	ScriptListModel *local_scripts_model;
};

#endif // SCRIPTBROWSERDIALOG_H
