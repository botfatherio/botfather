#include "config_dialog.h"
#include "ui_config_dialog.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QDebug>
#include "../shared/settings.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ConfigDialog)
{
	ui->setupUi(this);
	this->loadConfig();

	QSettings settings;
	QString flash_filename = settings.value(options::browser::FLASH_FILENAME).toString();

	QFileInfo flash_fileinfo(flash_filename);
	if (flash_fileinfo.exists() && flash_fileinfo.isFile()) {
		ui->manual_flash_installation_info->setText("<p style='color:green'>Manual flash installation found!</p>");
	} else {
		ui->manual_flash_installation_info->setText("<p style='color:red'>No manual flash installation found.</p>");
	}
	
	connect(ui->button_box, SIGNAL(accepted()), this, SLOT(saveConfig()));
	connect(ui->button_box, SIGNAL(accepted()), this, SLOT(close()));
	connect(ui->button_box, SIGNAL(rejected()), this, SLOT(close()));
}

ConfigDialog::~ConfigDialog()
{
	delete ui;
}

void ConfigDialog::saveConfig()
{
	QSettings s;
	s.setValue(options::browser::WIDTH, this->ui->browser_width->value());
	s.setValue(options::browser::HEIGHT, this->ui->browser_height->value());
	s.setValue(options::browser::FLASH_FILENAME, this->ui->local_flash_filename->text());
	s.setValue(options::browser::FLASH_VERSION, this->ui->local_flash_version->text());
	s.setValue(options::browser::USE_SYSTEM_FLASH, this->ui->use_system_flash->isChecked());
	s.setValue(options::android::ADB_BINARY, this->ui->adb_binary->text());
}

void ConfigDialog::loadConfig()
{
	QSettings s;
	this->ui->browser_width->setValue(s.value(options::browser::WIDTH, fallback::browser::WIDTH).toInt());
	this->ui->browser_height->setValue(s.value(options::browser::HEIGHT, fallback::browser::HEIGHT).toInt());
	this->ui->local_flash_filename->setText(s.value(options::browser::FLASH_FILENAME).toString());
	this->ui->local_flash_version->setText(s.value(options::browser::FLASH_VERSION).toString());
	this->ui->use_system_flash->setChecked(s.value(options::browser::USE_SYSTEM_FLASH, fallback::browser::USE_SYSTEM_FLASH).toBool());
	this->ui->adb_binary->setText(s.value(options::android::ADB_BINARY).toString());
}

void ConfigDialog::on_adb_binary_browse_button_clicked()
{
	QString adb_binary_file_name = QFileDialog::getOpenFileName(
		this,
		tr("Select Android Debug Bridge (ADB) Executable"),
		"",
		tr("Executable (*.exe, *)"),
		Q_NULLPTR,
		QFileDialog::DontUseNativeDialog
	);
	
	if (!adb_binary_file_name.isEmpty()) {
		ui->adb_binary->setText(adb_binary_file_name);
	}
}