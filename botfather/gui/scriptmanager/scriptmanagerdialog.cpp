#include "scriptmanagerdialog.h"
#include "ui_scriptmanagerdialog.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QMessageBox>
#include "gitdialog.h"

ScriptManagerDialog::ScriptManagerDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ScriptManagerDialog)
{
	ui->setupUi(this);
	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	// Set up the install tab and online scripts model

	online_scripts_model = new ScriptListModel(this);
	online_scripts_proxy= new QSortFilterProxyModel(this);
	online_scripts_proxy->setSourceModel(online_scripts_model);

	online_scripts_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
	online_scripts_proxy->setFilterRole(ScriptListModel::KeywordsRole);
	online_scripts_proxy->setFilterKeyColumn(0);

	ui->online_view->setModel(online_scripts_proxy);
	connect(ui->online_filter, &QLineEdit::textChanged, online_scripts_proxy, &QSortFilterProxyModel::setFilterWildcard);
	connect(ui->online_view, &QTableView::doubleClicked, this, &ScriptManagerDialog::itemDoubleClicked);

	// FIXME: get script data from the website instead
	QVector<RemoteScript> testscripts = {
		RemoteScript("Elisa Music Player", "KDE", "https://anongit.kde.org/elisa.git", "Elisa is a music player developed by the KDE community that strives to be simple and nice to use."),
		RemoteScript("Clementine", "Clementine Team", "https://github.com/clementine-player/Clementine.git", "Clementine is a multiplatform music player."),
	};
	for (ScriptRepository ts : testscripts)
	{
		online_scripts_model->addEntry(ts);
	}

	// Set up the manage tab and the local scripts model

	local_scripts_model = new ScriptListModel(this);
	local_scripts_proxy = new QSortFilterProxyModel(this);
	local_scripts_proxy->setSourceModel(local_scripts_model);

	local_scripts_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
	local_scripts_proxy->setFilterRole(ScriptListModel::KeywordsRole);
	local_scripts_proxy->setFilterKeyColumn(0);

	ui->local_view->setModel(local_scripts_proxy);
	connect(ui->local_filter, &QLineEdit::textChanged, local_scripts_proxy, &QSortFilterProxyModel::setFilterWildcard);

	// TODO: dont block the constructor
	local_scripts_model->load("local_scripts.dat"); // FIXME: store the local_scripts.dat at a standard location
}

ScriptManagerDialog::~ScriptManagerDialog()
{
	local_scripts_model->save("local_scripts.dat");
	delete ui;
}

void ScriptManagerDialog::itemDoubleClicked(const QModelIndex &index)
{
	if (!index.isValid())
	{
		return;
	}

	QString repo_dir_path = QFileDialog::getExistingDirectory(
		this,
		tr("Open Directory"),
		QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
		QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog
	);

	if (repo_dir_path.isEmpty())
	{
		qDebug() << "selected path empty";
		return;
	}

	QDir repo_dir(repo_dir_path);
	if (!repo_dir.isEmpty(QDir::AllEntries|QDir::NoDotAndDotDot))
	{
		// The choosen directory must be empty. Otherwise git_clone will fail.
		// FIXME: show qmessagebox to inform the user
		qDebug() << "dir not empty";
		return;
	}

	GitDialog *dialog = new GitDialog(this);
	connect(dialog, &GitDialog::cloned, local_scripts_model, &ScriptListModel::addEntry);

	ScriptRepository repository = qvariant_cast<ScriptRepository>(online_scripts_model->data(index, ScriptListModel::NativeDataRole));
	dialog->clone(repository, repo_dir_path);
}
