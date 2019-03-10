#include "scriptmanagerdialog.h"
#include "ui_scriptmanagerdialog.h"
#include <QMessageBox>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QInputDialog>
#include <QUuid>
#include <QDir>
#include <QThread>
#include <QAbstractButton>
#include <QPushButton>
#include <QDebug>
#include "gitprogressdialog.h"
#include "../../auth/scriptsapiclient.h"

ScriptManagerDialog::ScriptManagerDialog(QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::ScriptManagerDialog)
{
	m_ui->setupUi(this);
	connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	m_repos_model = new BotRepoListModel(this);
	m_repos_proxy= new QSortFilterProxyModel(this);
	m_repos_proxy->setSourceModel(m_repos_model);

	m_repos_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
	m_repos_proxy->setFilterRole(BotRepoListModel::KeywordsRole);
	m_repos_proxy->setFilterKeyColumn(0);

	m_ui->view->setModel(m_repos_proxy);
	m_ui->view->hideColumn(3); // Hide the Url column

	connect(m_ui->filter, &QLineEdit::textChanged, m_repos_proxy, &QSortFilterProxyModel::setFilterWildcard);
	connect(m_ui->view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(accept())); // Will trigger the line below and close the dialog
	connect(m_ui->view->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &ScriptManagerDialog::handleRowChange);
	connect(this, SIGNAL(accepted()), this, SLOT(installSelectedScript()));

	m_more_info_button = m_ui->buttonBox->addButton("More info", QDialogButtonBox::ButtonRole::HelpRole);
	connect(m_more_info_button, &QPushButton::clicked, this, &ScriptManagerDialog::giveMoreInfo);
	connect(m_more_info_button, &QPushButton::clicked, m_more_info_button, &QPushButton::clearFocus);
	m_more_info_button->setDisabled(true);

	QPushButton *refresh_button = m_ui->buttonBox->addButton("Refresh", QDialogButtonBox::ButtonRole::ResetRole);
	connect(refresh_button, &QPushButton::clicked, this, &ScriptManagerDialog::refetchModelData);
	connect(refresh_button, &QPushButton::clicked, refresh_button, &QPushButton::clearFocus);

	// Load model data initially
	refetchModelData();
}

ScriptManagerDialog::~ScriptManagerDialog()
{
	delete m_ui;
}

void ScriptManagerDialog::handleRowChange(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(previous);
	m_more_info_button->setEnabled(current.isValid());
}

void ScriptManagerDialog::refetchModelData()
{
	// Clear the model view
	if (m_repos_model->rowCount() > 0) {
		m_repos_model->removeRows(0, m_repos_model->rowCount());
	}

	QThread *sac_thread = new QThread;
	sac_thread->setObjectName("Scriptlist fetch thread");

	ScriptsApiClient *sac = new ScriptsApiClient;
	sac->moveToThread(sac_thread);

	connect(sac_thread, SIGNAL(started()), sac, SLOT(requestScripts()));
	connect(sac, SIGNAL(finished()), sac_thread, SLOT(quit()));
	connect(sac, SIGNAL(finished()), sac, SLOT(deleteLater()));
	connect(sac_thread, SIGNAL(finished()), sac_thread, SLOT(deleteLater()));

	connect(sac, &ScriptsApiClient::errorsReceived, [this](){
		// This error implies that our REST API returned errors and is likely broken
		m_ui->status_label->setText("<span style='color:red'>Fetching the script list failed. Contact support.</span>");
	});

	connect(sac, &ScriptsApiClient::networkError, [this](){
		m_ui->status_label->setText("<span style='color:red'>Fetching the script list failed. Check your network.</span>");
	});

	connect(sac, &ScriptsApiClient::finished, m_ui->progressBar, &QProgressBar::hide);
	connect(sac, &ScriptsApiClient::scriptsReceived, m_repos_model, &BotRepoListModel::addBotRepos);
	sac_thread->start();
}

void ScriptManagerDialog::installSelectedScript()
{
	QModelIndex current = m_ui->view->selectionModel()->currentIndex();
	if (!current.isValid()) return;

	BotRepo bot_repo = qvariant_cast<BotRepo>(m_repos_model->data(current, BotRepoListModel::NativeDataRole));

	QInputDialog *name_dialog = new QInputDialog(this);
	name_dialog->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	name_dialog->setInputMode(QInputDialog::TextInput);
	name_dialog->resize(380, 100);
	name_dialog->setWindowTitle("Choose a bot name");
	name_dialog->setLabelText("Please choose a fancy bot name");
	name_dialog->setTextValue(bot_repo.name()); // As default name

	if (!name_dialog->exec() || name_dialog->textValue().isEmpty())
	{
		// Dialog closed or provided text is empty
		return;
	}

	// Create the bot dir // TODO: make this an extra method

	QDir bot_parent_dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	bot_parent_dir.mkpath(bot_parent_dir.absolutePath());

	QString bot_name = name_dialog->textValue();
	QString bot_uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);

	if (!bot_parent_dir.mkdir(bot_uuid))
	{
		// Fatal error, couldn't create the repo dir
		return;
	}

	QString bot_path = bot_parent_dir.filePath(bot_uuid);

	Bot::Data bot_data(bot_path, bot_name, bot_repo.gitUrl());
	qDebug() << "Bot created. Path:" << bot_data.path << "Name:" << bot_data.name << "Repo:" << bot_data.repo;

	cloneRepository(bot_data);
}

void ScriptManagerDialog::cloneRepository(const Bot::Data &bot_data)
{
	GitProgressDialog *dialog = new GitProgressDialog(this);
	connect(dialog, &GitProgressDialog::cloned, [this, bot_data](){
		emit botCreated(bot_data);
	});
	dialog->clone(bot_data.repo, bot_data.path);
}

void ScriptManagerDialog::giveMoreInfo()
{
	QModelIndex current = m_ui->view->selectionModel()->currentIndex();
	if (!current.isValid()) return;

	BotRepo bot_repo = qvariant_cast<BotRepo>(m_repos_model->data(current, BotRepoListModel::NativeDataRole));
	QDesktopServices::openUrl(bot_repo.scriptUrl());
}
