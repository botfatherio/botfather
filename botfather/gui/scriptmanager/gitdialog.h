#ifndef GITDIALOG_H
#define GITDIALOG_H

#include <QProgressDialog>

class GitDialog : public QProgressDialog
{
	Q_OBJECT

public:
	explicit GitDialog(QWidget *parent = nullptr);

public slots:
	void clone(const QString &repo_url, const QString &dir_path);
	//void update();
	void transferProgressChanged(uint received, uint total, uint bytes);
	void checkoutProgressChanged(ulong current, ulong total, const QString &path);
};

#endif // GITDIALOG_H
