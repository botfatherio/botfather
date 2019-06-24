#include "browser_util.hpp"

void BrowserUtil::runInMainThread(std::function<void()> callback)
{
	QTimer *timer = new QTimer;
	timer->setSingleShot(true);
	timer->moveToThread(QApplication::instance()->thread());

	QObject::connect(timer, &QTimer::timeout, [=]()
	{
		// This code will run in the main thread
		callback();
		timer->deleteLater();
	});

	QMetaObject::invokeMethod(timer, "start", Qt::QueuedConnection, Q_ARG(int, 0));
}

QByteArray BrowserUtil::convertCefBinaryValueToQByteArray(const CefRefPtr<CefBinaryValue> &cef_binary_value)
{
	size_t buffer_size = cef_binary_value->GetSize();
	char *buffer = new char[buffer_size];

	cef_binary_value->GetData(buffer, buffer_size, 0);
	QByteArray byte_array(buffer, buffer_size);

	delete[] buffer;
	return byte_array;
}
