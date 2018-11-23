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

private:
	Ui::ScriptManagerDialog *ui;
};

#endif // SCRIPTBROWSERDIALOG_H
