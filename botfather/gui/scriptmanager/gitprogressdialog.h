#ifndef GITPROGRESSDIALOG_H
#define GITPROGRESSDIALOG_H

#include <QDialog>
#include "models/scriptrepository.h"

namespace Ui {
class GitProgressDialog;
}

class GitProgressDialog : public QDialog
{
	Q_OBJECT

public:
	explicit GitProgressDialog(QWidget *parent = nullptr);
	~GitProgressDialog();

public slots:
	void setLabelText(const QString &text);
	void setMaximum(int maximum);
	void setValue(int value);

	void transferProgressChanged(uint received, uint total, uint bytes);
	void checkoutProgressChanged(ulong current, ulong total, const QString &path);

	void reclone(ScriptRepository *repository);
	void clone(ScriptRepository *repository);
	void cloneSuccess();
	void cloneFailure();

signals:
	void cloned(ScriptRepository *repository);

private:
	Ui::GitProgressDialog *m_ui;
	ScriptRepository *m_dest_repo;
};

#endif // GITPROGRESSDIALOG_H
