#ifndef SCRIPTMANAGERDIALOG_H
#define SCRIPTMANAGERDIALOG_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include "models/scriptreposmodel.h"

namespace Ui {
class ScriptManagerDialog;
}

class ScriptManagerDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ScriptManagerDialog(QWidget *parent = nullptr);
	~ScriptManagerDialog();

signals:
	void executeRepository(ScriptRepository *repository);

private:
	Ui::ScriptManagerDialog *m_ui;
};

#endif // SCRIPTMANAGERDIALOG_H
