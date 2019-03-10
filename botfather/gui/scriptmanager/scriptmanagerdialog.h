#ifndef SCRIPTMANAGERDIALOG_H
#define SCRIPTMANAGERDIALOG_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include "../models/botrepolistmodel.h"
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
	void handleRowChange(const QModelIndex &current, const QModelIndex &previous);
	void refetchModelData();
	void installSelectedScript();
	void cloneRepository(const Bot::Data &bot_data);
	void giveMoreInfo();

signals:
	void botCreated(const Bot::Data &bot_data);

private:
	Ui::ScriptManagerDialog *m_ui;
	QPushButton *m_more_info_button;
	BotRepoListModel *m_repos_model;
	QSortFilterProxyModel *m_repos_proxy;
};

#endif // SCRIPTMANAGERDIALOG_H
