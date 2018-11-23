#include "scriptmanagewidget.h"
#include "ui_scriptlistwidget.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QTimer>
#include <git2.h>
#include "gitprogressdialog.h"

ScriptManageWidget::ScriptManageWidget(QWidget *parent)
	: QWidget(parent)
	, m_ui(new Ui::ScriptListWidget)
{
	m_ui->setupUi(this);
	m_ui->label->setText("<p style='font-size: 14px'>Update and run local Scripts</p>");

	// Keep track of installed scripts in a "scriptrepos.dat" file
	QDir app_config_dir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
	app_config_dir.mkpath(app_config_dir.path());
	m_scripts_dat_filepath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/scriptrepos.dat";

	m_scripts_model = new ScriptListModel(this);
	m_scripts_proxy = new QSortFilterProxyModel(this);
	m_scripts_proxy->setSourceModel(m_scripts_model);

	m_scripts_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
	m_scripts_proxy->setFilterRole(ScriptListModel::KeywordsRole);
	m_scripts_proxy->setFilterKeyColumn(0);

	m_ui->view->setModel(m_scripts_proxy);
	connect(m_ui->filter, &QLineEdit::textChanged, m_scripts_proxy, &QSortFilterProxyModel::setFilterWildcard);
	connect(m_ui->view->selectionModel(), &QItemSelectionModel::currentChanged, this, &ScriptManageWidget::changeLocalButtonTarget);

	m_update_button = new QPushButton("Update", this);
	m_inspect_button = new QPushButton("Inspect", this);
	m_delete_button = new QPushButton("Delete", this);

	m_update_button->setDisabled(true);
	m_inspect_button->setDisabled(true);
	m_delete_button->setDisabled(true);

	m_ui->buttons->addWidget(m_update_button);
	m_ui->buttons->addWidget(m_inspect_button);
	m_ui->buttons->addWidget(m_delete_button);

	connect(m_update_button, &QPushButton::clicked, this, &ScriptManageWidget::updateSelectedLocalRepository);
	connect(m_inspect_button, &QPushButton::clicked, this, &ScriptManageWidget::inspectSelectedLocalRepository);
	connect(m_delete_button, &QPushButton::clicked, this, &ScriptManageWidget::deleteSelectedLocalRepository);

	// Don't block the constructor while loading model data
	QTimer::singleShot(1, this, &ScriptManageWidget::loadLocalModelData);
}

ScriptManageWidget::~ScriptManageWidget()
{
	m_scripts_model->save(m_scripts_dat_filepath);
	delete m_ui;
}

void ScriptManageWidget::loadLocalModelData()
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

		// Pass nullptr for the output parameter to check for but not open the repo
		if (git_repository_open_ext(nullptr, repo_date.local_path.toUtf8(), GIT_REPOSITORY_OPEN_NO_SEARCH, nullptr) == 0)
		{
			qDebug() << "Adding repository to local model:" << repo_date.local_path;
			m_scripts_model->addEntry(new ScriptRepository(repo_date));
		}
	}

	git_libgit2_shutdown();
	file.close();
}

void ScriptManageWidget::changeLocalButtonTarget(const QModelIndex &current, const QModelIndex &previous)
{
	ScriptRepository *repository = qvariant_cast<ScriptRepository*>(m_scripts_model->data(current, ScriptListModel::NativeDataRole));
	m_update_button->setEnabled(current.isValid() && repository->status() == ScriptRepository::Status::Outdated);
	m_inspect_button->setEnabled(current.isValid());
	m_delete_button->setEnabled(current.isValid());
}

void ScriptManageWidget::updateSelectedLocalRepository()
{
	QModelIndex current = m_ui->view->selectionModel()->currentIndex();
	if (!current.isValid()) return;

	ScriptRepository *repository = qvariant_cast<ScriptRepository*>(m_scripts_model->data(current, ScriptListModel::NativeDataRole));
	qDebug() << "Lets update" << repository->localPath();

	if (repository->status() != ScriptRepository::Status::Outdated)
	{
		// Only outdated repositories can be updated
		return;
	}

	GitProgressDialog *dialog = new GitProgressDialog(this);
	connect(dialog, SIGNAL(accepted()), this, SLOT(localRepositoryUpdated()));
	dialog->reclone(repository);
}

void ScriptManageWidget::localRepositoryUpdated()
{
	QModelIndex current = m_ui->view->selectionModel()->currentIndex();
	if (!current.isValid()) return;

	ScriptRepository *repository = qvariant_cast<ScriptRepository*>(m_scripts_model->data(current, ScriptListModel::NativeDataRole));
	connect(repository, &ScriptRepository::statusChanged, [this, current](){
		changeLocalButtonTarget(current, current);
	});

	repository->checkStatus();
}

void ScriptManageWidget::inspectSelectedLocalRepository()
{
	QModelIndex current = m_ui->view->selectionModel()->currentIndex();
	if (!current.isValid()) return;

	ScriptRepository *repository = qvariant_cast<ScriptRepository*>(m_scripts_model->data(current, ScriptListModel::NativeDataRole));
	qDebug() << "Opening" << repository->localPath();

	QDesktopServices::openUrl(QUrl::fromLocalFile(repository->localPath()));
}

void ScriptManageWidget::deleteSelectedLocalRepository()
{
	QModelIndex current = m_ui->view->selectionModel()->currentIndex();
	if (!current.isValid()) return;

	ScriptRepository *repository = qvariant_cast<ScriptRepository*>(m_scripts_model->data(current, ScriptListModel::NativeDataRole));
	qDebug() << "Deleting" << repository->localPath();

	QDir repo_dir(repository->localPath());
	if (repo_dir.removeRecursively())
	{
		m_scripts_model->removeRows(current.row(), 1);
	}
}

void ScriptManageWidget::addScriptRepository(ScriptRepository *repository)
{
	m_scripts_model->addEntry(repository);
}
