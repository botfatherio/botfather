#include "repodownloadwidget.h"
#include "ui_repolistwidget.h"
#include <QMessageBox>
#include <QStandardPaths>
#include <QInputDialog>
#include <QTimer>
#include <QUuid>
#include <QDir>
#include <QDebug>
#include "gitprogressdialog.h"

RepoDownloadWidget::RepoDownloadWidget(QWidget *parent)
	: QWidget(parent)
	, m_ui(new Ui::RepoListWidget)
{
	m_ui->setupUi(this);
	m_ui->label->setText("<p style='font-size: 14px'>Download and install Scripts</p>");

	m_repos_model = new ScriptReposModel(this);
	m_repos_proxy= new QSortFilterProxyModel(this);
	m_repos_proxy->setSourceModel(m_repos_model);

	m_repos_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
	m_repos_proxy->setFilterRole(ScriptReposModel::KeywordsRole);
	m_repos_proxy->setFilterKeyColumn(0);

	m_ui->view->setModel(m_repos_proxy);
	connect(m_ui->filter, &QLineEdit::textChanged, m_repos_proxy, &QSortFilterProxyModel::setFilterWildcard);
	connect(m_ui->view, &QTableView::doubleClicked, this, &RepoDownloadWidget::itemDoubleClicked);

	// Hide some columns after setting the model
	m_ui->view->hideColumn(1); // Status
	m_ui->view->hideColumn(4); // Local Path
	m_ui->view->hideColumn(5); // Remote Url

	// Don't block the constructor while loading model data
	QTimer::singleShot(1, this, &RepoDownloadWidget::loadModelData);
}

RepoDownloadWidget::~RepoDownloadWidget()
{
	delete m_ui;
}

void RepoDownloadWidget::loadModelData()
{
	// TODO: get script data from the website instead
	QVector<ScriptRepository*> testscripts = {
		new ScriptRepository({"Elisa Music Player", "KDE", "Elisa is a music player developed by the KDE community that strives to be simple and nice to use.", "", "https://anongit.kde.org/elisa.git"}),
		new ScriptRepository({"Clementine", "Clementine Team", "Clementine is a multiplatform music player.", "", "https://github.com/clementine-player/Clementine.git"}),
		new ScriptRepository({"Fetch Test Repo", "DoctorJohn", "No description", "", "https://github.com/DoctorJohn/tmp-test-repo.git"}),
	};
	for (ScriptRepository *ts : testscripts)
	{
		m_repos_model->addEntry(ts);
	}
}

void RepoDownloadWidget::itemDoubleClicked(const QModelIndex &index)
{
	if (!index.isValid()) return;
	ScriptRepository *repository = qvariant_cast<ScriptRepository*>(m_repos_model->data(index, ScriptReposModel::NativeDataRole));

	QDir repo_parent_dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	repo_parent_dir.mkpath(repo_parent_dir.absolutePath());

	//QString reponame = QInputDialog::getText(this, "Choose a bot name", "Please choose a fancy bot name");
	//if (reponame.isEmpty()) return;

	QInputDialog *name_dialog = new QInputDialog(this);
	name_dialog->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	name_dialog->setInputMode(QInputDialog::TextInput);
	name_dialog->resize(380, 100);
	name_dialog->setWindowTitle("Choose a bot name");
	name_dialog->setLabelText("Please choose a fancy bot name");

	if (!name_dialog->exec() || name_dialog->textValue().isEmpty())
	{
		// Dialog closed or provided text is empty
		return;
	}

	QString reponame = name_dialog->textValue();
	QString repouuid = QUuid::createUuid().toString(QUuid::WithoutBraces);

	if (!repo_parent_dir.mkdir(repouuid))
	{
		// Fatal error, couldn't create the repo dir
		return;
	}

	QString repopath = repo_parent_dir.filePath(repouuid);
	qDebug() << "Repodir created. Name:" << reponame << "Uuid:" << repouuid << "Path:" << repopath;

	ScriptRepository *new_repo = new ScriptRepository(repository->data());
	new_repo->setName(reponame);
	new_repo->setLocalPath(repopath);

	cloneRepository(new_repo);
}

void RepoDownloadWidget::cloneRepository(ScriptRepository *repository)
{
	GitProgressDialog *dialog = new GitProgressDialog(this);
	connect(dialog, &GitProgressDialog::cloned, this, &RepoDownloadWidget::scriptInstalled);
	dialog->clone(repository);
}
