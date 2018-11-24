#include "scriptmanagerdialog.h"
#include "ui_scriptmanagerdialog.h"
#include "repodownloadwidget.h"
#include "repomaintainwidget.h"
#include <QDebug>

ScriptManagerDialog::ScriptManagerDialog(QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::ScriptManagerDialog)
{
	m_ui->setupUi(this);
	connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	RepoDownloadWidget *install_widget = new RepoDownloadWidget(this);
	RepoMaintainWidget *manage_widget = new RepoMaintainWidget(this);

	m_ui->install_tab->layout()->addWidget(install_widget);
	m_ui->manage_tab->layout()->addWidget(manage_widget);

	connect(install_widget, &RepoDownloadWidget::scriptInstalled, manage_widget, &RepoMaintainWidget::addScriptRepository);
	connect(manage_widget, SIGNAL(executeRepository(ScriptRepository*)), this, SLOT(accept()));
	connect(manage_widget, SIGNAL(executeRepository(ScriptRepository*)), this, SIGNAL(executeRepository(ScriptRepository*)));
}

ScriptManagerDialog::~ScriptManagerDialog()
{
	delete m_ui;
}
