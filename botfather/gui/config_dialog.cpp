#include "config_dialog.h"
#include "ui_config_dialog.h"
#include <QSettings>
#include <QFileInfo>
#include "../shared/constants.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ConfigDialog)
{
	ui->setupUi(this);
	this->loadConfig();

	QSettings settings;
	QString flash_filename = settings.value("LOCAL_FLASH_FILENAME", constants::LOCAL_FLASH_FILENAME).toString();

	QFileInfo flash_fileinfo(flash_filename);
	if (flash_fileinfo.exists() && flash_fileinfo.isFile()) {
		ui->manual_flash_installation_info->setText("<p style='color:green'>Manual flash installation found!</p>");
	} else {
		ui->manual_flash_installation_info->setText("<p style='color:red'>No manual flash installation found.</p>");
	}
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
	s.setValue("LOCAL_FLASH_FILENAME", this->ui->local_flash_filename->text());
	s.setValue("LOCAL_FLASH_VERSION", this->ui->local_flash_version->text());
	s.setValue("USE_SYSTEM_FLASH", this->ui->use_system_flash->isChecked());
}

void ConfigDialog::loadConfig()
{
	QSettings s;
	this->ui->browser_width->setValue(s.value("BROWSER_WIDTH", constants::BROWSER_WIDTH).toInt());
	this->ui->browser_height->setValue(s.value("BROWSER_HEIGHT", constants::BROWSER_HEIGHT).toInt());
	this->ui->local_flash_filename->setText(s.value("LOCAL_FLASH_FILENAME", constants::LOCAL_FLASH_FILENAME).toString());
	this->ui->local_flash_version->setText(s.value("LOCAL_FLASH_VERSION", constants::LOCAL_FLASH_VERSION).toString());
	this->ui->use_system_flash->setChecked(s.value("USE_SYSTEM_FLASH", constants::USE_SYSTEM_FLASH).toBool());
}