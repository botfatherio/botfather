#include "android_dialog.h"
#include "ui_android_dialog.h"
#include <QRadioButton>
#include <QDebug>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include "../shared/settings.h"
#include "../android/adb_wrapper.h"

AndroidDialog::AndroidDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AndroidDialog)
{
	ui->setupUi(this);
	ui->attached_devices->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	QPushButton *refresh_button = ui->button_box->addButton("Refresh", QDialogButtonBox::ActionRole);
	
	connect(refresh_button, SIGNAL(clicked()), this, SLOT(refreshListOfDevicesAttached()));
	connect(ui->button_box, SIGNAL(accepted()), this, SLOT(save()));
	connect(ui->button_box, SIGNAL(accepted()), this, SLOT(close()));
	connect(ui->button_box, SIGNAL(rejected()), this, SLOT(close()));
}

AndroidDialog::~AndroidDialog()
{
	delete ui;
}

void AndroidDialog::refreshListOfDevicesAttached()
{
	ui->attached_devices->clearContents();
	ui->attached_devices->setRowCount(0);
	
	QSettings settings;
	QString adb_binary = settings.value(options::android::ADB_BINARY).toString();
	AdbWrapper adb(this, adb_binary);
	
	QList<AdbDeviceInfo> devices;
	if (!adb.queryForDevices(devices)) {
		QString message(
			"Can't use ADB. Botfather was unable to use the Android Debug Bridge."
			"Please check the Botfather Android settings and make sure you've set the correct path to your ADB executable."
			"The path we tried to use: " + adb_binary
		);
		QMessageBox::warning(this, "Can't use ADB", message);
		return;
	}
	
	if (devices.isEmpty()) {
		QString message(
			"No devices attached!\n"
			"If your are using a real Android device make sure it's properly connected to your computer via USB.\n"
			"Also make sure Developer Option are enabled on your device and you gave your computer permission to control it.\n"
			"If your are using an emulator make sure that it automatically connects to the Android Debug Bridge."
		);
		QMessageBox::information(this, "No devices attached", message);
		return;
	}
	
	for (AdbDeviceInfo device : devices) {
		ui->attached_devices->insertRow(ui->attached_devices->rowCount());
		
		QTableWidgetItem *serial_number_item = new QTableWidgetItem(device.getSerialNumber());
		ui->attached_devices->setItem(ui->attached_devices->rowCount() - 1, 0, serial_number_item);
		
		QTableWidgetItem *product_item = new QTableWidgetItem(device.getProduct());
		ui->attached_devices->setItem(ui->attached_devices->rowCount() - 1, 1, product_item);
		
		QTableWidgetItem *model_item = new QTableWidgetItem(device.getModel());
		ui->attached_devices->setItem(ui->attached_devices->rowCount() - 1, 2, model_item);
	}
}

void AndroidDialog::save()
{
	int row = ui->attached_devices->currentRow();
	QTableWidgetItem *item = ui->attached_devices->item(row, 0);
	QString serial_number = item ? item->text() : "";
	
	QSettings settings;
	settings.setValue(options::android::SERIAL_NUMBER, serial_number);
}