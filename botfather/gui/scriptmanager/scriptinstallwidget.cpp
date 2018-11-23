#include "scriptinstallwidget.h"
#include "ui_scriptlistwidget.h"
#include <QMessageBox>
#include <QTimer>
#include <QStandardPaths>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include "gitprogressdialog.h"

ScriptInstallWidget::ScriptInstallWidget(QWidget *parent)
	: QWidget(parent)
	, m_ui(new Ui::ScriptListWidget)
{
	m_ui->setupUi(this);
	m_ui->label->setText("<p style='font-size: 14px'>Download and install Scripts</p>");

	m_scripts_model = new ScriptListModel(this);
	m_scripts_proxy= new QSortFilterProxyModel(this);
	m_scripts_proxy->setSourceModel(m_scripts_model);

	m_scripts_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
	m_scripts_proxy->setFilterRole(ScriptListModel::KeywordsRole);
	m_scripts_proxy->setFilterKeyColumn(0);

	m_ui->view->setModel(m_scripts_proxy);
	connect(m_ui->filter, &QLineEdit::textChanged, m_scripts_proxy, &QSortFilterProxyModel::setFilterWildcard);
	connect(m_ui->view, &QTableView::doubleClicked, this, &ScriptInstallWidget::itemDoubleClicked);

	// Hide some columns after setting the model
	m_ui->view->hideColumn(1); // Status
	m_ui->view->hideColumn(4); // Local Path
	m_ui->view->hideColumn(5); // Remote Url

	// Don't block the constructor while loading model data
	QTimer::singleShot(1, this, &ScriptInstallWidget::loadModelData);
}

ScriptInstallWidget::~ScriptInstallWidget()
{
	delete m_ui;
}

void ScriptInstallWidget::loadModelData()
{
	// TODO: get script data from the website instead
	QVector<ScriptRepository*> testscripts = {
		new ScriptRepository({"Elisa Music Player", "KDE", "Elisa is a music player developed by the KDE community that strives to be simple and nice to use.", "", "https://anongit.kde.org/elisa.git"}),
		new ScriptRepository({"Clementine", "Clementine Team", "Clementine is a multiplatform music player.", "", "https://github.com/clementine-player/Clementine.git"}),
		new ScriptRepository({"Fetch Test Repo", "DoctorJohn", "No description", "", "https://github.com/DoctorJohn/tmp-test-repo.git"}),
	};
	for (ScriptRepository *ts : testscripts)
	{
		m_scripts_model->addEntry(ts);
	}
}

void ScriptInstallWidget::itemDoubleClicked(const QModelIndex &index)
{
	if (!index.isValid()) return;

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

	GitProgressDialog *dialog = new GitProgressDialog(this);
	connect(dialog, &GitProgressDialog::cloned, this, &ScriptInstallWidget::scriptInstalled);

	ScriptRepository *repository = qvariant_cast<ScriptRepository*>(m_scripts_model->data(index, ScriptListModel::NativeDataRole));
	dialog->clone(repository, repo_dir_path);
}
