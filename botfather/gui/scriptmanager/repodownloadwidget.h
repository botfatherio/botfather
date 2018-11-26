#ifndef REPODOWNLOADWIDGET_H
#define REPODOWNLOADWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include "models/scriptreposmodel.h"

namespace Ui {
class RepoListWidget;
}

class RepoDownloadWidget : public QWidget
{
	Q_OBJECT

public:
	explicit RepoDownloadWidget(QWidget *parent = nullptr);
	~RepoDownloadWidget();

public slots:
	void loadModelData(const QVector<ScriptRepository::Data> &repo_data_list);
	void updateButtonStatuses(const QModelIndex &current, const QModelIndex &previous);
	void installSelectedScript();
	void cloneRepository(ScriptRepository *repository);

signals:
	void scriptInstalled(ScriptRepository *repository);

private:
	Ui::RepoListWidget *m_ui;
	QPushButton *m_install_button;
	ScriptReposModel *m_repos_model;
	QSortFilterProxyModel *m_repos_proxy;
};

#endif // REPODOWNLOADWIDGET_H
