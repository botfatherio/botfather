#ifndef BFP__GUI__BROWSER_WINDOW_H
#define BFP__GUI__BROWSER_WINDOW_H

#include <QMainWindow>
#include <QImage>
#include "browser_widget.h"
#include "browser_address_bar.h"

namespace Ui {
	class BrowserWindow;
}

class BrowserWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit BrowserWindow(QWidget *parent = nullptr);
	~BrowserWindow();

	BrowserWidget *browserWidget();
	
public slots:
	void show();
	void paintSlot(QImage browser_image);
	
private:
	Ui::BrowserWindow *ui;
	BrowserWidget* pixmap_placeholder;
	BrowserAddressBar* addressbar;
};

#endif // BFP__GUI__BROWSER_WINDOW_H
