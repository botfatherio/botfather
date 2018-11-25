#ifndef REPOMAINTAINWIDGET_H
#define REPOMAINTAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include "models/scriptreposmodel.h"

namespace Ui {
class RepoListWidget;
}

class RepoMaintainWidget : public QWidget
{
	Q_OBJECT

public:
	explicit RepoMaintainWidget(QWidget *parent = nullptr);
	~RepoMaintainWidget();

public slots:
	void loadModelData();
	void updateButtonStatuses(const QModelIndex &current, const QModelIndex &previous);
	void executeSelectedRepository();
	void inspectSelectedRepository();
	void updateSelectedRepository();
	void repositoryUpdated();
	void deleteSelectedRepository();
	void addScriptRepository(ScriptRepository *repository);
	void runLocalScript();

signals:
	void scriptExecRequest(const QString &script_path);

private:
	Ui::RepoListWidget *m_ui;
	QPushButton *m_execute_button;
	QPushButton *m_inspect_button;
	QPushButton *m_update_button;
	QPushButton *m_delete_button;

	ScriptReposModel *m_repos_model;
	QSortFilterProxyModel *m_repos_proxy;
	QString m_scripts_dat_filepath;
};

#endif // REPOMAINTAINWIDGET_H
