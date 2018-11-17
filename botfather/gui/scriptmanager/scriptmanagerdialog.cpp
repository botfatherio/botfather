#include "scriptmanagerdialog.h"
#include "ui_scriptmanagerdialog.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
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

	// Set up the manage tab and the local scripts model

	local_scripts_model = new ScriptListModel(this);
	local_scripts_proxy = new QSortFilterProxyModel(this);
	local_scripts_proxy->setSourceModel(local_scripts_model);

	local_scripts_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
	local_scripts_proxy->setFilterRole(ScriptListModel::KeywordsRole);
	local_scripts_proxy->setFilterKeyColumn(0);

	ui->local_view->setModel(local_scripts_proxy);
	connect(ui->local_filter, &QLineEdit::textChanged, local_scripts_proxy, &QSortFilterProxyModel::setFilterWildcard);
	connect(ui->local_view->selectionModel(), &QItemSelectionModel::currentChanged, this, &ScriptManagerDialog::changeLocalButtonTarget);
	connect(ui->update_button, &QPushButton::clicked, this, &ScriptManagerDialog::updateSelectedLocalRepository);
	connect(ui->inspect_button, &QPushButton::clicked, this, &ScriptManagerDialog::inspectSelectedLocalRepository);
	connect(ui->delete_button, &QPushButton::clicked, this, &ScriptManagerDialog::deleteSelectedLocalRepository);

	// Don't block the constructor while loading model data
	QTimer::singleShot(1, this, &ScriptManagerDialog::loadModelData);
}

ScriptManagerDialog::~ScriptManagerDialog()
{
	local_scripts_model->save("local_scripts.dat");
	delete ui;
}

void ScriptManagerDialog::loadModelData()
{
	local_scripts_model->load("local_scripts.dat"); // FIXME: store the local_scripts.dat at a standard location

	// FIXME: get script data from the website instead
	QVector<RemoteScript> testscripts = {
		RemoteScript("Elisa Music Player", "KDE", "https://anongit.kde.org/elisa.git", "Elisa is a music player developed by the KDE community that strives to be simple and nice to use."),
		RemoteScript("Clementine", "Clementine Team", "https://github.com/clementine-player/Clementine.git", "Clementine is a multiplatform music player."),
	};
	for (ScriptRepository ts : testscripts)
	{
		online_scripts_model->addEntry(ts);
	}
}

void ScriptManagerDialog::changeLocalButtonTarget(const QModelIndex &current, const QModelIndex &previous)
{
	ui->update_button->setEnabled(current.isValid());
	ui->inspect_button->setEnabled(current.isValid());
	ui->delete_button->setEnabled(current.isValid());
}

void ScriptManagerDialog::updateSelectedLocalRepository()
{
	QModelIndex current = ui->local_view->selectionModel()->currentIndex();
	if (!current.isValid()) return;

	ScriptRepository repository = qvariant_cast<ScriptRepository>(local_scripts_model->data(current, ScriptListModel::NativeDataRole));
	qDebug() << "Lets update" << repository.repository();

	// TODO: Actually update the script
}

void ScriptManagerDialog::inspectSelectedLocalRepository()
{
	QModelIndex current = ui->local_view->selectionModel()->currentIndex();
	if (!current.isValid()) return;

	ScriptRepository repository = qvariant_cast<ScriptRepository>(local_scripts_model->data(current, ScriptListModel::NativeDataRole));
	qDebug() << "Opening" << repository.repository();

	QDesktopServices::openUrl(QUrl::fromLocalFile(repository.repository()));
}

void ScriptManagerDialog::deleteSelectedLocalRepository()
{
	QModelIndex current = ui->local_view->selectionModel()->currentIndex();
	if (!current.isValid()) return;

	ScriptRepository repository = qvariant_cast<ScriptRepository>(local_scripts_model->data(current, ScriptListModel::NativeDataRole));
	qDebug() << "Deleting" << repository.repository();

	QDir repo_dir(repository.repository());
	if (repo_dir.removeRecursively())
	{
		local_scripts_model->removeRows(current.row(), 1);
	}
}

void ScriptManagerDialog::itemDoubleClicked(const QModelIndex &index)
{
	if (!index.isValid())
	{
		return;
	}

	QDir dir;
	dir.mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));

	QFileDialog file_dialog(
		this,
		tr("Where to store the script?"),
		QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
	);

	file_dialog.setOptions(QFileDialog::ShowDirsOnly/*|QFileDialog::DontUseNativeDialog*/);
	file_dialog.setFileMode(QFileDialog::Directory);
	file_dialog.setViewMode(QFileDialog::Detail);

	if (!file_dialog.exec()) return;
	QString repo_dir_path = file_dialog.selectedFiles().first();

	QDir repo_dir(repo_dir_path);
	if (!repo_dir.isEmpty(QDir::AllEntries|QDir::NoDotAndDotDot))
	{
		QMessageBox::information(this, "Directory not empty", "The selected directory must be empty.");
		return; // Otherwise git_clone will fail.
	}

	GitDialog *dialog = new GitDialog(this);
	connect(dialog, &GitDialog::cloned, local_scripts_model, &ScriptListModel::addEntry);

	ScriptRepository repository = qvariant_cast<ScriptRepository>(online_scripts_model->data(index, ScriptListModel::NativeDataRole));
	dialog->clone(repository, repo_dir_path);
}
