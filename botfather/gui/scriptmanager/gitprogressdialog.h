#ifndef GITPROGRESSDIALOG_H
#define GITPROGRESSDIALOG_H

#include <QDialog>
#include "../models/botrepo.h"

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

	void reclone(const QString &repo_url, const QString &dir_path);
	void clone(const QString &repo_url, const QString &dir_path);
	void cloneSuccess();
	void cloneFailure();

signals:
	void cloned(); // TODO: maybe rename this signal

private:
	Ui::GitProgressDialog *m_ui;
};

#endif // GITPROGRESSDIALOG_H
