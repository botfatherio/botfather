#include "scriptmanagerdialog.h"
#include "ui_scriptmanagerdialog.h"
#include "scriptinstallwidget.h"
#include "scriptmanagewidget.h"
#include <QDebug>

ScriptManagerDialog::ScriptManagerDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ScriptManagerDialog)
{
	ui->setupUi(this);
	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	ScriptInstallWidget *install_widget = new ScriptInstallWidget(this);
	ScriptManageWidget *manage_widget = new ScriptManageWidget(this);

	ui->install_tab->layout()->addWidget(install_widget);
	ui->manage_tab->layout()->addWidget(manage_widget);

	connect(install_widget, &ScriptInstallWidget::scriptInstalled, manage_widget, &ScriptManageWidget::addScriptRepository);
}

ScriptManagerDialog::~ScriptManagerDialog()
{
	delete ui;
}
