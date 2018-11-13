#include "scriptmanagerdialog.h"
#include "ui_scriptmanagerdialog.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include "gitdialog.h"

ScriptManagerDialog::ScriptManagerDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ScriptManagerDialog)
{
	ui->setupUi(this);
	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	script_list_model = new ScriptListModel(this);
	proxy_model = new QSortFilterProxyModel(this);
	//proxy_model->setSourceModel(script_list_model);
	ui->tableView->setModel(proxy_model);

	local_scripts_model = new ScriptListModel(this);
	local_scripts_model->load("local_scripts.dat"); // FIXME: store the local_scripts.dat at a standard location
	QSortFilterProxyModel *local_scripts_proxy_model = new QSortFilterProxyModel(this);
	local_scripts_proxy_model->setSourceModel(local_scripts_model);
	ui->local_scripts_table_view->setModel(local_scripts_proxy_model);

	connect(ui->search_button, &QPushButton::clicked, ui->search_input, &QLineEdit::selectAll);
	connect(ui->search_input, &QLineEdit::textChanged, this, &ScriptManagerDialog::search);
	connect(ui->tableView, &QTableView::doubleClicked, this, &ScriptManagerDialog::itemDoubleClicked);

	ui->search_input->setFocus();

	// FIXME: get script data from the website instead
	QVector<RemoteScript> testscripts = {
		RemoteScript("Elisa Music Player", "KDE", "https://anongit.kde.org/elisa.git", "Elisa is a music player developed by the KDE community that strives to be simple and nice to use."),
		RemoteScript("Clementine", "Clementine Team", "https://github.com/clementine-player/Clementine.git", "Clementine is a multiplatform music player."),
	};
	for (RemoteScript ts : testscripts)
	{
		script_list_model->addRemoteScript(ts);
	}

	proxy_model->setSourceModel(script_list_model);
}

ScriptManagerDialog::~ScriptManagerDialog()
{
	local_scripts_model->save("local_scripts.dat");
	delete ui;
}

void ScriptManagerDialog::search()
{
	QString search_query = ui->search_input->text();
	proxy_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
	proxy_model->setFilterRole(ScriptListModel::KeywordsRole);
	proxy_model->setFilterKeyColumn(0);
	proxy_model->setFilterWildcard(search_query);
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

	RemoteScript remote_script = qvariant_cast<RemoteScript>(proxy_model->data(index, ScriptListModel::NativeDataRole));

	GitDialog *dialog = new GitDialog(this);
	dialog->clone(remote_script.repository, repo_dir_path);

	// TODO: show message box to inform the user. (use the git clone return code)
	// Why? Cloning can be so fast that the user won't see the progress dialog a all
	// TODO: Save the script location on clone success, so that it can be managed

	RemoteScript local_script = remote_script;
	local_script.repository = repo_dir_path;
	local_scripts_model->addRemoteScript(local_script);
}
