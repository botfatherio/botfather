#ifndef SCRIPTMANAGERDIALOG_H
#define SCRIPTMANAGERDIALOG_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include "../models/scriptreposmodel.h"
#include "../models/bot.h"

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
	void loadModelData(const QVector<ScriptRepository::Data> &repo_data_list);
	void installSelectedScript();
	void cloneRepository(const Bot::Data &bot_data);

signals:
	void botCreated(const Bot::Data &bot_data);

private:
	Ui::ScriptManagerDialog *m_ui;
	ScriptReposModel *m_repos_model;
	QSortFilterProxyModel *m_repos_proxy;
};

#endif // SCRIPTMANAGERDIALOG_H
