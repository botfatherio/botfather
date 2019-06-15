#include "adb_device_info.hpp"

AdbDeviceInfo::AdbDeviceInfo()
{
	
}

AdbDeviceInfo::AdbDeviceInfo(QString serial_number, QString state, QString product, QString model)
{
	this->serial_number = serial_number;
	this->state = state;
	this->product = product;
	this->model = model;
}

QString AdbDeviceInfo::getSerialNumber() const
{
	return serial_number;
}

void AdbDeviceInfo::setSerialNumber(const QString &serial_number)
{
	this->serial_number = serial_number;
}

QString AdbDeviceInfo::getState() const
{
	return state;
}

void AdbDeviceInfo::setState(const QString &state)
{
	this->state = state;
}

QString AdbDeviceInfo::getProduct() const
{
	return product;
}

void AdbDeviceInfo::setProduct(const QString &product)
{
	this->product = product;
}

QString AdbDeviceInfo::getModel() const
{
	return model;
}

void AdbDeviceInfo::setModel(const QString &model)
{
	this->model = model;
}

bool AdbDeviceInfo::connected() const
{
	return state == "device";
}
