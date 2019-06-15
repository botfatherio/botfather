#include "preferences_dialog.h"
#include "ui_preferences_dialog.h"
#include <QFileDialog>
#include <QDebug>
#include <QSettings>
#include "../maintenancetool/maintenancetool.h"
#include "../../engine/modules/android/android_settings.h"
#include "../../engine/modules/browser/browser_settings.h"

PreferencesDialog::PreferencesDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::PreferencesDialog)
{
	ui->setupUi(this);

	connect(ui->button_box, SIGNAL(accepted()), this, SLOT(saveConfig()));
	connect(ui->button_box, SIGNAL(accepted()), this, SLOT(close()));
	connect(ui->button_box, SIGNAL(rejected()), this, SLOT(close()));

#ifdef Q_OS_LINUX
	ui->flash_on_window->hide();
#else
	ui->flash_on_linux->hide();
#endif

	ui->check_for_updates->setDisabled(MaintenanceTool::filePath().isEmpty());

	// Disabled/enable the flash input fields with the de/activation of the 'use custom flash' option.
	connect(ui->use_bundled_flash, SIGNAL(toggled(bool)), ui->flash_so, SLOT(setDisabled(bool)));
	connect(ui->use_bundled_flash, SIGNAL(toggled(bool)), ui->flash_manifest, SLOT(setDisabled(bool)));
	connect(ui->use_bundled_flash, SIGNAL(toggled(bool)), ui->browse_flash_so, SLOT(setDisabled(bool)));
	connect(ui->use_bundled_flash, SIGNAL(toggled(bool)), ui->browse_flash_manifest, SLOT(setDisabled(bool)));

	connect(ui->use_bundled_adb, SIGNAL(toggled(bool)), ui->adb_binary, SLOT(setDisabled(bool)));
	connect(ui->use_bundled_adb, SIGNAL(toggled(bool)), ui->browse_adb_binary, SLOT(setDisabled(bool)));

	// Load the config after connecting the gui up, so that elements can notify each other about values set.
	loadConfig();
}

PreferencesDialog::~PreferencesDialog()
{
	delete ui;
}

void PreferencesDialog::saveConfig()
{
	QSettings s;
	s.setValue(browser::options::WIDTH, ui->browser_width->value());
	s.setValue(browser::options::HEIGHT, ui->browser_height->value());

	s.setValue(browser::options::FLASH_SO, ui->flash_so->text());
	s.setValue(browser::options::FLASH_MANIFEST, ui->flash_manifest->text());
	s.setValue(browser::options::USE_SYSTEM_FLASH, ui->use_system_flash->isChecked());
	s.setValue(browser::options::USE_CUSTOM_FLASH, ui->use_custom_flash->isChecked());
	s.setValue(browser::options::USE_BUNDLED_FLASH, ui->use_bundled_flash->isChecked());

	s.setValue(android::options::ADB_BINARY, ui->adb_binary->text());
	s.setValue(android::options::USE_CUSTOM_ADB, ui->use_custom_adb->isChecked());
	s.setValue(android::options::USE_BUNDLED_ADB, ui->use_bundled_adb->isChecked());

	s.setValue("check_for_updates", ui->check_for_updates->isChecked());

	emit configSaved();
}

void PreferencesDialog::loadConfig()
{
	QSettings s;

	// Put the defaults last. If the config is broken they will be set last and thus used.

	ui->browser_width->setValue(s.value(browser::options::WIDTH, browser::fallback::WIDTH).toInt());
	ui->browser_height->setValue(s.value(browser::options::HEIGHT, browser::fallback::HEIGHT).toInt());

	ui->flash_so->setText(s.value(browser::options::FLASH_SO).toString());
	ui->flash_manifest->setText(s.value(browser::options::FLASH_MANIFEST).toString());
	ui->use_custom_flash->setChecked(s.value(browser::options::USE_CUSTOM_FLASH, browser::fallback::USE_CUSTOM_FLASH).toBool());
	ui->use_system_flash->setChecked(s.value(browser::options::USE_SYSTEM_FLASH, browser::fallback::USE_SYSTEM_FLASH).toBool());
	ui->use_bundled_flash->setChecked(s.value(browser::options::USE_BUNDLED_FLASH, browser::fallback::USE_BUNDLED_FLASH).toBool());

	ui->adb_binary->setText(s.value(android::options::ADB_BINARY).toString());
	ui->use_custom_adb->setChecked(s.value(android::options::USE_CUSTOM_ADB, android::fallback::USE_CUSTOM_ADB).toBool());
	ui->use_bundled_adb->setChecked(s.value(android::options::USE_BUNDLED_ADB, android::fallback::USE_BUNDLED_ADB).toBool());

	ui->check_for_updates->setChecked(s.value("check_for_updates", true).toBool() && !MaintenanceTool::filePath().isEmpty());

	emit configLoaded();
}

void PreferencesDialog::on_browse_adb_binary_pressed()
{
	QString adb_binary_file_name = QFileDialog::getOpenFileName(
		this,
		tr("Select Android Debug Bridge (ADB) Executable"),
		"",
		tr("Executable (*.exe, *)"),
		Q_NULLPTR
	);
	
	if (!adb_binary_file_name.isEmpty()) {
		ui->adb_binary->setText(adb_binary_file_name);
	}
}

void PreferencesDialog::on_browse_flash_so_pressed()
{
	QString flash_so = QFileDialog::getOpenFileName(
		this,
		tr("Select Flashplayer for Opera and Chromium PPAPI"),
		"",
		tr("Flashplayer PPAPI (libpepflashplayer.so)"),
		Q_NULLPTR
	);
	
	if (!flash_so.isEmpty()) {
		ui->flash_so->setText(flash_so);
	}
}

void PreferencesDialog::on_browse_flash_manifest_pressed()
{
	QString flash_manifest = QFileDialog::getOpenFileName(
		this,
		tr("Select Flashplayer PPAPI Manifest"),
		"",
		tr("FP PPAPI Manifest (manifest.json)"),
		Q_NULLPTR
	);
	
	if (!flash_manifest.isEmpty()) {
		ui->flash_manifest->setText(flash_manifest);
	}
}
