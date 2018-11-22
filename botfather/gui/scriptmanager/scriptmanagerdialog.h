#ifndef SCRIPTBROWSERDIALOG_H
#define SCRIPTBROWSERDIALOG_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include "scriptlistmodel.h"

namespace Ui {
class ScriptManagerDialog;
}

class ScriptManagerDialog : public QDialog // TODO: make this a widget/window. Dialogs have unwished defaults (enter closes dialog)
{
	Q_OBJECT

public:
	explicit ScriptManagerDialog(QWidget *parent = nullptr);
	~ScriptManagerDialog();

public slots:
	void loadLocalModelData();
	void loadOnlineModelData();
	void changeLocalButtonTarget(const QModelIndex &current, const QModelIndex &previous);
	void updateSelectedLocalRepository();
	void localRepositoryUpdated();
	void inspectSelectedLocalRepository();
	void deleteSelectedLocalRepository();
	void itemDoubleClicked(const QModelIndex &index);

private:
	Ui::ScriptManagerDialog *ui;
	ScriptListModel *online_scripts_model;
	ScriptListModel *local_scripts_model;
	QSortFilterProxyModel *online_scripts_proxy;
	QSortFilterProxyModel *local_scripts_proxy;
	QString m_scripts_dat_filepath;
};

#endif // SCRIPTBROWSERDIALOG_H
