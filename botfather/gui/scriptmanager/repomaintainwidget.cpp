#include "repomaintainwidget.h"
#include "ui_repolistwidget.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QTimer>
#include <git2.h>
#include "gitprogressdialog.h"

RepoMaintainWidget::RepoMaintainWidget(QWidget *parent)
	: QWidget(parent)
	, m_ui(new Ui::RepoListWidget)
{
	m_ui->setupUi(this);
	m_ui->label->setText("<p style='font-size: 14px'>Update and run local Scripts</p>");

	// Keep track of installed scripts in a "scriptrepos.dat" file
	QDir app_config_dir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
	app_config_dir.mkpath(app_config_dir.path());
	m_scripts_dat_filepath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/scriptrepos.dat";

	m_repos_model = new ScriptReposModel(this);
	m_repos_proxy = new QSortFilterProxyModel(this);
	m_repos_proxy->setSourceModel(m_repos_model);

	m_repos_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
	m_repos_proxy->setFilterRole(ScriptReposModel::KeywordsRole);
	m_repos_proxy->setFilterKeyColumn(0);

	m_ui->view->setModel(m_repos_proxy);
	connect(m_ui->filter, &QLineEdit::textChanged, m_repos_proxy, &QSortFilterProxyModel::setFilterWildcard);
	connect(m_ui->view->selectionModel(), &QItemSelectionModel::currentChanged, this, &RepoMaintainWidget::updateButtonStatuses);

	m_update_button = new QPushButton("Update", this);
	m_inspect_button = new QPushButton("Inspect", this);
	m_delete_button = new QPushButton("Delete", this);

	m_update_button->setDisabled(true);
	m_inspect_button->setDisabled(true);
	m_delete_button->setDisabled(true);

	m_ui->buttons->addWidget(m_update_button);
	m_ui->buttons->addWidget(m_inspect_button);
	m_ui->buttons->addWidget(m_delete_button);
	m_ui->buttons->addStretch();

	connect(m_update_button, &QPushButton::clicked, this, &RepoMaintainWidget::updateSelectedRepository);
	connect(m_inspect_button, &QPushButton::clicked, this, &RepoMaintainWidget::inspectSelectedRepository);
	connect(m_delete_button, &QPushButton::clicked, this, &RepoMaintainWidget::deleteSelectedRepository);

	// Don't block the constructor while loading model data
	QTimer::singleShot(1, this, &RepoMaintainWidget::loadModelData);
}

RepoMaintainWidget::~RepoMaintainWidget()
{
	m_repos_model->save(m_scripts_dat_filepath);
	delete m_ui;
}

void RepoMaintainWidget::loadModelData()
{
	QFile file(m_scripts_dat_filepath);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	QVector<ScriptRepository::Data> repo_data;
	QDataStream in(&file);
	in >> repo_data;

	git_libgit2_init();

	for (ScriptRepository::Data repo_date : repo_data)
	{
		// TODO: move this check and use of libgit into the ScriptRepository::isValid method
		// NOTE: after doing so move this back into the model

		// Pass nullptr for the output parameter to check for but not open the repo
		if (git_repository_open_ext(nullptr, repo_date.local_path.toUtf8(), GIT_REPOSITORY_OPEN_NO_SEARCH, nullptr) == 0)
		{
			qDebug() << "Adding repository to local model:" << repo_date.local_path;
			m_repos_model->addEntry(new ScriptRepository(repo_date));
		}
	}

	git_libgit2_shutdown();
	file.close();
}

void RepoMaintainWidget::updateButtonStatuses(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(previous);
	ScriptRepository *repository = qvariant_cast<ScriptRepository*>(m_repos_model->data(current, ScriptReposModel::NativeDataRole));
	m_update_button->setEnabled(current.isValid() && repository->status() == ScriptRepository::Status::Outdated);
	m_inspect_button->setEnabled(current.isValid());
	m_delete_button->setEnabled(current.isValid());
}

void RepoMaintainWidget::updateSelectedRepository()
{
	QModelIndex current = m_ui->view->selectionModel()->currentIndex();
	if (!current.isValid()) return;

	ScriptRepository *repository = qvariant_cast<ScriptRepository*>(m_repos_model->data(current, ScriptReposModel::NativeDataRole));
	qDebug() << "Lets update" << repository->localPath();

	if (repository->status() != ScriptRepository::Status::Outdated)
	{
		// Only outdated repositories can be updated
		return;
	}

	GitProgressDialog *dialog = new GitProgressDialog(this);
	connect(dialog, SIGNAL(accepted()), this, SLOT(repositoryUpdated()));
	dialog->reclone(repository);
}

void RepoMaintainWidget::repositoryUpdated()
{
	QModelIndex current = m_ui->view->selectionModel()->currentIndex();
	if (!current.isValid()) return;

	ScriptRepository *repository = qvariant_cast<ScriptRepository*>(m_repos_model->data(current, ScriptReposModel::NativeDataRole));
	connect(repository, &ScriptRepository::statusChanged, [this, current](){
		updateButtonStatuses(current, current);
	});

	repository->checkStatus();
}

void RepoMaintainWidget::inspectSelectedRepository()
{
	QModelIndex current = m_ui->view->selectionModel()->currentIndex();
	if (!current.isValid()) return;

	ScriptRepository *repository = qvariant_cast<ScriptRepository*>(m_repos_model->data(current, ScriptReposModel::NativeDataRole));
	qDebug() << "Opening" << repository->localPath();

	QDesktopServices::openUrl(QUrl::fromLocalFile(repository->localPath()));
}

void RepoMaintainWidget::deleteSelectedRepository()
{
	QModelIndex current = m_ui->view->selectionModel()->currentIndex();
	if (!current.isValid()) return;

	ScriptRepository *repository = qvariant_cast<ScriptRepository*>(m_repos_model->data(current, ScriptReposModel::NativeDataRole));
	qDebug() << "Deleting" << repository->localPath();

	QDir repo_dir(repository->localPath());
	if (repo_dir.removeRecursively())
	{
		m_repos_model->removeRows(current.row(), 1);
	}
}

void RepoMaintainWidget::addScriptRepository(ScriptRepository *repository)
{
	m_repos_model->addEntry(repository);
}
