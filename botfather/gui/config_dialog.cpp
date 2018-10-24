#include "config_dialog.h"
#include "ui_config_dialog.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QDebug>
#include <QSettings>
#include "../engine/modules/android/android_settings.h"
#include "../engine/modules/browser/browser_settings.h"
#include "../settings.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ConfigDialog)
{
	ui->setupUi(this);
	loadConfig();

	QSettings settings;
	QString flash_so = settings.value(browser::options::FLASH_SO).toString();
	QString flash_manifest = settings.value(browser::options::FLASH_MANIFEST).toString();

	QFileInfo flash_so_info(flash_so);
	if (!flash_so_info.exists() || !flash_so_info.isFile()) {
		// TODO: eventually show an error message or get rid of it entirely
	}
	
	QFileInfo flash_manifest_info(flash_manifest);
	if (!flash_manifest_info.exists() || !flash_manifest_info.isFile()) {
		// TODO: eventually show an error message or get rid of it entirely
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
	s.setValue(browser::options::WIDTH, ui->browser_width->value());
	s.setValue(browser::options::HEIGHT, ui->browser_height->value());
	s.setValue(browser::options::FLASH_SO, ui->flash_so->text());
	s.setValue(browser::options::FLASH_MANIFEST, ui->flash_manifest->text());
	s.setValue(browser::options::USE_SYSTEM_FLASH, ui->use_system_flash->isChecked());
	s.setValue(android::options::ADB_BINARY, ui->adb_binary->text());
	s.setValue(general::options::STOP_SHORTCUT, ui->stop_shortcut->keySequence().toString());
	s.setValue(general::options::DEVMODE, ui->devmode->isChecked());
	emit configSaved();
}

void ConfigDialog::loadConfig()
{
	QSettings s;
	ui->browser_width->setValue(s.value(browser::options::WIDTH, browser::fallback::WIDTH).toInt());
	ui->browser_height->setValue(s.value(browser::options::HEIGHT, browser::fallback::HEIGHT).toInt());
	ui->flash_so->setText(s.value(browser::options::FLASH_SO).toString());
	ui->flash_manifest->setText(s.value(browser::options::FLASH_MANIFEST).toString());
	ui->use_system_flash->setChecked(s.value(browser::options::USE_SYSTEM_FLASH, browser::fallback::USE_SYSTEM_FLASH).toBool());
	ui->adb_binary->setText(s.value(android::options::ADB_BINARY).toString());
	ui->stop_shortcut->setKeySequence(QKeySequence::fromString(s.value(general::options::STOP_SHORTCUT).toString()));
	ui->devmode->setChecked(s.value(general::options::DEVMODE, general::fallback::DEVMODE).toBool());
	emit configLoaded();
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
