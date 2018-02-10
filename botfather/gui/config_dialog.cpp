#include "config_dialog.h"
#include "ui_config_dialog.h"
#include <QSettings>
#include "../shared/constants.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ConfigDialog)
{
	ui->setupUi(this);
	this->loadConfig();
}

ConfigDialog::~ConfigDialog()
{
	delete ui;
}

void ConfigDialog::on_cancel_clicked()
{
	this->close();
}

void ConfigDialog::on_okay_clicked()
{
	this->saveConfig();
	this->close();
}

void ConfigDialog::saveConfig()
{
	QSettings s;
	s.setValue("BROWSER_WIDTH", this->ui->browser_width->value());
	s.setValue("BROWSER_HEIGHT", this->ui->browser_height->value());
	s.setValue("BROWSER_RENDER", this->ui->browser_render->isChecked());
	s.setValue("OPEN_LOG_ON_PLAY", this->ui->open_log_on_play->isChecked());
}

void ConfigDialog::loadConfig()
{
	QSettings s;
	this->ui->browser_width->setValue(s.value("BROWSER_WIDTH", constants::BROWSER_WIDTH).toInt());
	this->ui->browser_height->setValue(s.value("BROWSER_HEIGHT", constants::BROWSER_HEIGHT).toInt());
	this->ui->browser_render->setChecked(s.value("BROWSER_RENDER", constants::BROWSER_RENDER).toBool());
	this->ui->open_log_on_play->setChecked(s.value("OPEN_LOG_ON_PLAY", constants::OPEN_LOG_ON_PLAY).toBool());
}