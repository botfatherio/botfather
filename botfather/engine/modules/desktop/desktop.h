#ifndef BFP_ENGINE_MODULES_DESKTOP_DESKTOP_H
#define BFP_ENGINE_MODULES_DESKTOP_DESKTOP_H

#include <QObject>
#include <QString>
#include <QImage>
#include <QScopedPointer>

class DesktopPrivate;

class Desktop : public QObject
{
	Q_OBJECT
public:
	Desktop(QObject *parent = nullptr);
	~Desktop();
	
	QImage takeScreenshot();
	QSize getSize();

	void leftClick(const QPoint &position);
	void middleClick(const QPoint &position);
	void rightClick(const QPoint &position);

	void pressKey(const QString &key);
	void holdKey(const QString &key);
	void releaseKey(const QString &key);

	void warpCursor(const QPoint &position);
	bool getCursorPosition(int *x, int *y);

	bool keyExists(const QString &key);
	
private:
	QScopedPointer<DesktopPrivate> const pimpl;
	Q_DECLARE_PRIVATE(Desktop)
};

#endif // BFP_ENGINE_MODULES_DESKTOP_DESKTOP_H
