#include "browser_util.h"

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
