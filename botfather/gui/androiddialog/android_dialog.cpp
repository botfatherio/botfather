#include "android_dialog.hpp"

#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSettings>
#include <QStandardPaths>
#include <QTableWidgetItem>

#include "../../engine/android/adb_wrapper.hpp"
#include "../../engine/android/android_settings.hpp"
#include "ui_android_dialog.h"

AndroidDialog::AndroidDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AndroidDialog) {
    ui->setupUi(this);
    ui->attached_devices->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
    QPushButton *refresh_button =
        ui->button_box->addButton("Refresh", QDialogButtonBox::ActionRole);

    connect(refresh_button, SIGNAL(clicked()), this,
            SLOT(refreshListOfDevicesAttached()));
    connect(ui->button_box, SIGNAL(accepted()), this, SLOT(save()));
    connect(ui->button_box, SIGNAL(accepted()), this, SLOT(close()));
    connect(ui->button_box, SIGNAL(rejected()), this, SLOT(close()));
}

AndroidDialog::~AndroidDialog() { delete ui; }

void AndroidDialog::refreshListOfDevicesAttached() {
    ui->attached_devices->clearContents();
    ui->attached_devices->setRowCount(0);

    QStringList search_paths;
    search_paths << QApplication::applicationDirPath()
                 << QDir(QApplication::applicationDirPath()).filePath("adb");

    QString adb_binary =
        QStandardPaths::findExecutable("adb", search_paths);  // default
    QSettings settings;

    if (settings
            .value(android::options::USE_CUSTOM_ADB,
                   android::fallback::USE_CUSTOM_ADB)
            .toBool()) {
        adb_binary = settings.value(android::options::ADB_BINARY).toString();
    }

    AdbWrapper adb(this, adb_binary);
    adb.startAdbServer();

    QVector<AdbDeviceInfo> devices;
    if (!adb.queryForDevices(devices)) {
        QString message(
            "Can't use ADB.\nBotfather was unable to use the Android Debug "
            "Bridge.\n"
            "Please check the Botfather Android settings and make sure you've "
            "set the correct path to your ADB executable.\n"
            "The path we tried to use: " +
            adb_binary);
        QMessageBox::warning(this, "Can't use ADB", message);
        return;
    }

    if (devices.isEmpty()) {
        QString message(
            "No devices attached!\n"
            "If your are using a real Android device make sure it's properly "
            "connected to your computer via USB.\n"
            "Also make sure Developer Option are enabled on your device and "
            "you gave your computer permission to control it.\n"
            "If your are using an emulator make sure that it automatically "
            "connects to the Android Debug Bridge.");
        QMessageBox::information(this, "No devices attached", message);
        return;
    }

    for (AdbDeviceInfo device : devices) {
        ui->attached_devices->insertRow(ui->attached_devices->rowCount());

        QTableWidgetItem *serial_number_item =
            new QTableWidgetItem(device.getSerialNumber());
        ui->attached_devices->setItem(ui->attached_devices->rowCount() - 1, 0,
                                      serial_number_item);

        QTableWidgetItem *product_item =
            new QTableWidgetItem(device.getProduct());
        ui->attached_devices->setItem(ui->attached_devices->rowCount() - 1, 1,
                                      product_item);

        QTableWidgetItem *model_item = new QTableWidgetItem(device.getModel());
        ui->attached_devices->setItem(ui->attached_devices->rowCount() - 1, 2,
                                      model_item);
    }
}

void AndroidDialog::save() {
    int row = ui->attached_devices->currentRow();
    QTableWidgetItem *item = ui->attached_devices->item(row, 0);
    QString serial_number = item ? item->text() : "";

    QSettings settings;
    settings.setValue(android::options::SERIAL_NUMBER, serial_number);
}
