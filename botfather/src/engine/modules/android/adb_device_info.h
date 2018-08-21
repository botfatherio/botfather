#ifndef BFP_ENGINE_MODULES_ANDROID_ADB_DEVICE_INFO_H
#define BFP_ENGINE_MODULES_ANDROID_ADB_DEVICE_INFO_H

#include <QString>

class AdbDeviceInfo
{
public:
	AdbDeviceInfo();
	AdbDeviceInfo(QString serial_number, QString state, QString product, QString model);
	
	QString getSerialNumber() const;
	void setSerialNumber(QString serial_number);
	
	QString getState() const;
	void setState(QString state);
	
	QString getProduct() const;
	void setProduct(QString product);
	
	QString getModel() const;
	void setModel(QString model);
	
	bool connected() const;
	
private:
	// A string created by adb to uniquely identify the device by its port number. Here's an example serial number: emulator-5554
	QString serial_number;
	
	// The connection state of the device can be one of the following: 
	// - offline: The device is not connected to adb or is not responding.
    // - device: The device is now connected to the adb server. Note that this state does not imply that the Android system is fully
	//   booted and operational because the device connects to adb while the system is still booting.
	//   However, after boot-up, this is the normal operational state of an device.
    // - no device: There is no device connected. 
	QString state;
	
	// Eg: razor
	QString product;
	
	// Eg: Nexus_7
	QString model;
};

#endif // BFP_ENGINE_MODULES_ANDROID_ADB_DEVICE_INFO_H