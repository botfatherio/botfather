#ifndef REPODOWNLOADWIDGET_H
#define REPODOWNLOADWIDGET_H

#include <QWidget>
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
	void loadModelData();
	void itemDoubleClicked(const QModelIndex &index);

signals:
	void scriptInstalled(ScriptRepository *repository);

private:
	Ui::RepoListWidget *m_ui;
	ScriptReposModel *m_repos_model;
	QSortFilterProxyModel *m_repos_proxy;
};

#endif // REPODOWNLOADWIDGET_H
