#ifndef BFP__DESKTOP__DESKTOP_H
#define BFP__DESKTOP__DESKTOP_H

#include <QObject>
#include <QString>
#include <QScopedPointer>
#include <opencv2/opencv.hpp>

class DesktopPrivate;

class Desktop : public QObject
{
	Q_OBJECT
public:
	Desktop(QObject *parent = nullptr);
	~Desktop();
	
	bool takeScreenshot(cv::UMat &screenshot);
	int getWidth();
	int getHeight();
	void leftClick(int x, int y);
	void middleClick(int x, int y);
	void rightClick(int x, int y);
	void pressKey(QString key);
	void holdKey(QString key);
	void releaseKey(QString key);
	void warpCursor(int x, int y);
	bool keyExists(QString key);
	
private:
	QScopedPointer<DesktopPrivate> const d_ptr;
	Q_DECLARE_PRIVATE(Desktop)
};

#endif // BFP__DESKTOP__DESKTOP_H
