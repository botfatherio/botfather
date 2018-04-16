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
	QString flash_so = settings.value(options::browser::FLASH_SO).toString();
	QString flash_manifest = settings.value(options::browser::FLASH_MANIFEST).toString();

	QFileInfo flash_so_info(flash_so);
	if (!flash_so_info.exists() || !flash_so_info.isFile()) {
		
	}
	
	QFileInfo flash_manifest_info(flash_manifest);
	if (!flash_manifest_info.exists() || !flash_manifest_info.isFile()) {
		
	}
	
#ifdef Q_OS_LINUX
    ui->flash_on_window->setEnabled(false);
#else
    ui->flash_on_linux->setEnabled(false);
#endif
	
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
	s.setValue(options::browser::FLASH_SO, this->ui->flash_so->text());
	s.setValue(options::browser::FLASH_MANIFEST, this->ui->flash_manifest->text());
	s.setValue(options::browser::USE_SYSTEM_FLASH, this->ui->use_system_flash->isChecked());
	s.setValue(options::android::ADB_BINARY, this->ui->adb_binary->text());
}

void ConfigDialog::loadConfig()
{
	QSettings s;
	this->ui->browser_width->setValue(s.value(options::browser::WIDTH, fallback::browser::WIDTH).toInt());
	this->ui->browser_height->setValue(s.value(options::browser::HEIGHT, fallback::browser::HEIGHT).toInt());
	this->ui->flash_so->setText(s.value(options::browser::FLASH_SO).toString());
	this->ui->flash_manifest->setText(s.value(options::browser::FLASH_MANIFEST).toString());
	this->ui->use_system_flash->setChecked(s.value(options::browser::USE_SYSTEM_FLASH, fallback::browser::USE_SYSTEM_FLASH).toBool());
	this->ui->adb_binary->setText(s.value(options::android::ADB_BINARY).toString());
}

void ConfigDialog::on_adb_binary_browse_button_pressed()
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

void ConfigDialog::on_browse_flash_so_pressed()
{
	QString flash_so = QFileDialog::getOpenFileName(
		this,
		tr("Select Flashplayer for Opera and Chromium PPAPI"),
		"",
		tr("Flashplayer PPAPI (libpepflashplayer.so)"),
		Q_NULLPTR,
		QFileDialog::DontUseNativeDialog
	);
	
	if (!flash_so.isEmpty()) {
		ui->flash_so->setText(flash_so);
	}
}

void ConfigDialog::on_browse_flash_manifest_pressed()
{
	QString flash_manifest = QFileDialog::getOpenFileName(
		this,
		tr("Select Flashplayer PPAPI Manifest"),
		"",
		tr("FP PPAPI Manifest (manifest.json)"),
		Q_NULLPTR,
		QFileDialog::DontUseNativeDialog
	);
	
	if (!flash_manifest.isEmpty()) {
		ui->flash_manifest->setText(flash_manifest);
	}
}