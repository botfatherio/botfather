#ifndef SCRIPTMANAGEWIDGET_H
#define SCRIPTMANAGEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include "scriptlistmodel.h"

namespace Ui {
class ScriptListWidget;
}

class ScriptManageWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ScriptManageWidget(QWidget *parent = nullptr);
	~ScriptManageWidget();

public slots:
	void loadLocalModelData();
	void changeLocalButtonTarget(const QModelIndex &current, const QModelIndex &previous);
	void updateSelectedLocalRepository();
	void localRepositoryUpdated();
	void inspectSelectedLocalRepository();
	void deleteSelectedLocalRepository();
	void addScriptRepository(ScriptRepository *repository);

private:
	Ui::ScriptListWidget *m_ui;
	QPushButton *m_update_button;
	QPushButton *m_inspect_button;
	QPushButton *m_delete_button;

	ScriptListModel *m_scripts_model;
	QSortFilterProxyModel *m_scripts_proxy;
	QString m_scripts_dat_filepath;
};

#endif // SCRIPTMANAGEWIDGET_H
