#include "gitdialog.h"
#include "gitprogress.h"
#include <git2.h>
#include <QDebug>

GitDialog::GitDialog(QWidget *parent) : QProgressDialog(parent)
{
	setMinimumWidth(240);
	git_libgit2_init();
}

GitDialog::~GitDialog()
{
	git_libgit2_shutdown();
}

void GitDialog::clone(const QString &repo_url, const QString &dir_path)
{
	setWindowTitle("Cloning script repository...");
	setLabelText("Cloning script repository...");
	setCancelButtonText("Cancel");
	show();

	git_repository *repo = nullptr;
	git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;

	GitProgress git_progress(this);
	connect(this, &GitDialog::canceled, &git_progress, &GitProgress::cancel);
	connect(&git_progress, &GitProgress::totalObjectsChanged, this, &GitDialog::setMaximum);
	connect(&git_progress, &GitProgress::receivedObjectsChanged, this, &GitDialog::setValue);
	connect(&git_progress, &GitProgress::checkoutTotalChanged, this, &GitDialog::setMaximum);
	connect(&git_progress, &GitProgress::checkoutCurrentChanged, this, &GitDialog::setValue);
	connect(&git_progress, &GitProgress::transferProgressChanged, this, &GitDialog::transferProgressChanged);
	connect(&git_progress, &GitProgress::checkoutProgressChanged, this, &GitDialog::checkoutProgressChanged);

	clone_opts.fetch_opts.callbacks.payload = &git_progress;
	clone_opts.fetch_opts.callbacks.transfer_progress = GitProgress::transferProgressCallback;
	clone_opts.checkout_opts.checkout_strategy = GIT_CHECKOUT_FORCE; // git checkout --force; all modifications are overwritten, and all missing files are created.
	clone_opts.checkout_opts.progress_cb = GitProgress::checkoutProgressCallback;
	clone_opts.checkout_opts.progress_payload = &git_progress;

	int code = git_clone(&repo, repo_url.toUtf8(), dir_path.toUtf8(), &clone_opts);
	qDebug() << "Git clone code:" << code << (code == 0 ? "" : giterr_last()->message);

	git_repository_free(repo);
}

void GitDialog::transferProgressChanged(uint received, uint total, uint bytes)
{
	Q_UNUSED(bytes)
	QString label_text = QString("Downloading objects: %1/%2 done.").arg(received).arg(total);
	setLabelText(label_text);
}

void GitDialog::checkoutProgressChanged(ulong current, ulong total, const QString &path)
{
	Q_UNUSED(path)
	QString label_text = QString("Checking out files: %1/%2 done.").arg(current).arg(total);
	setLabelText(label_text);
}
