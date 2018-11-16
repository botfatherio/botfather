#ifndef GITDIALOG_H
#define GITDIALOG_H

#include <QDialog>
#include "scriptrepository.h"

namespace Ui {
class GitDialog;
}

class GitDialog : public QDialog
{
	Q_OBJECT

public:
	explicit GitDialog(QWidget *parent = nullptr);
	~GitDialog();

public slots:
	void setLabelText(const QString &text);
	void setMaximum(int maximum);
	void setValue(int value);

	void transferProgressChanged(uint received, uint total, uint bytes);
	void checkoutProgressChanged(ulong current, ulong total, const QString &path);
	void clone(const ScriptRepository &repository, const QString &local_path);
	void cloneSuccess();
	void cloneFailure();

signals:
	void cloned(const ScriptRepository &repository);

private:
	Ui::GitDialog *ui;
	ScriptRepository m_repository;
};

#endif // GITDIALOG_H
