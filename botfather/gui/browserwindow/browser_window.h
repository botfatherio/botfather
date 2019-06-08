#ifndef BFP__GUI__BROWSER_WINDOW_H
#define BFP__GUI__BROWSER_WINDOW_H

#include <QMainWindow>
#include <QImage>

class BrowserWidget;
class BrowserAddressBar;

namespace Ui {
	class BrowserWindow;
}

class BrowserWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit BrowserWindow(QWidget *parent = nullptr);
	~BrowserWindow();
	
public slots:
	void show();
	void paintSlot(QImage browser_image);
	
signals:
	//void sendClick();
	//void sendUrl();

private:
	Ui::BrowserWindow *ui;
	BrowserWidget* pixmap_placeholder;
	BrowserAddressBar* addressbar;
};

#endif // BFP__GUI__BROWSER_WINDOW_H
