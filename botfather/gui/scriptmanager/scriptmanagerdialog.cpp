#include "scriptmanagerdialog.h"
#include "ui_scriptmanagerdialog.h"
#include <QMessageBox>
#include <QStandardPaths>
#include <QInputDialog>
#include <QUuid>
#include <QDir>
#include <QThread>
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
	connect(m_ui->filter, &QLineEdit::textChanged, m_repos_proxy, &QSortFilterProxyModel::setFilterWildcard);

	connect(m_ui->view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(accept())); // Will trigger the line below and close the dialog
	connect(this, SIGNAL(accepted()), this, SLOT(installSelectedScript()));

	// Hide some columns after setting the model
	m_ui->view->hideColumn(1); // Status
	m_ui->view->hideColumn(3); // Local Path // FIXME: remove this aswell
	m_ui->view->hideColumn(4); // Remote Url

	// Don't block the constructor while loading model data
	QThread *sac_thread = new QThread;
	sac_thread->setObjectName("Scriptlist fetch thread");

	ScriptsApiClient *sac = new ScriptsApiClient;
	sac->moveToThread(sac_thread);

	connect(sac_thread, SIGNAL(started()), sac, SLOT(requestScripts()));
	connect(sac, SIGNAL(finished()), sac_thread, SLOT(quit()));
	connect(sac, SIGNAL(finished()), sac, SLOT(deleteLater()));
	connect(sac_thread, SIGNAL(finished()), sac_thread, SLOT(deleteLater()));

	connect(sac, &ScriptsApiClient::errorsReceived, [](){
		qDebug() << "sac errors received"; // TODO: provide feedback to the user
	});

	connect(sac, &ScriptsApiClient::networkError, [](){
		qDebug() << "sac network error"; // TODO: provide feedback to the user
	});

	qRegisterMetaType<QVector<BotRepo::Data>>("QVector<BotRepo::Data>");

	connect(sac, &ScriptsApiClient::scriptsReceived, this, &ScriptManagerDialog::loadModelData);
	sac_thread->start();
}

ScriptManagerDialog::~ScriptManagerDialog()
{
	delete m_ui;
}

void ScriptManagerDialog::loadModelData(const QVector<BotRepo::Data> &repo_data_list)
{
	for (BotRepo::Data date : repo_data_list)
	{
		m_repos_model->addEntry(new BotRepo(date, m_repos_model));
	}
}

void ScriptManagerDialog::installSelectedScript()
{
	QModelIndex current = m_ui->view->selectionModel()->currentIndex();
	if (!current.isValid()) return;

	BotRepo *repository = qvariant_cast<BotRepo*>(m_repos_model->data(current, BotRepoListModel::NativeDataRole));

	QInputDialog *name_dialog = new QInputDialog(this);
	name_dialog->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	name_dialog->setInputMode(QInputDialog::TextInput);
	name_dialog->resize(380, 100);
	name_dialog->setWindowTitle("Choose a bot name");
	name_dialog->setLabelText("Please choose a fancy bot name");
	name_dialog->setTextValue(repository->name()); // As default text

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

	Bot::Data bot_data(bot_path, bot_name, repository->remoteUrl());
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
