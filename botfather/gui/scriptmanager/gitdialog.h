#ifndef GITDIALOG_H
#define GITDIALOG_H

#include <QDialog>

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
	void clone(const QString &repo_url, const QString &dir_path);
	void transferProgressChanged(uint received, uint total, uint bytes);
	void checkoutProgressChanged(ulong current, ulong total, const QString &path);

	void cloneSuccess();
	void cloneFailure();

private:
	Ui::GitDialog *ui;
};

#endif // GITDIALOG_H
