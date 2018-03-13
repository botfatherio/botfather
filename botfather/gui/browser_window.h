#ifndef BFP__GUI__BROWSER_WINDOW_H
#define BFP__GUI__BROWSER_WINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QTimer>

class BrowserWidget;

namespace Ui {
	class BrowserWindow;
}

class BrowserWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit BrowserWindow(QWidget *parent = 0);
	~BrowserWindow();
	
public slots:
	void applyResizing();
	void filterOldSize(QImage browser_image);
	void paintSlot(QImage browser_image);
	void on_actionHome_triggered();
	
private:
	Ui::BrowserWindow *ui;
	BrowserWidget* pixmap_placeholder;
	QTimer resize_timer;
	void showEvent(QShowEvent * event);
	void hideEvent(QHideEvent *event);
	void resizeEvent(QResizeEvent *event);
};

#endif // BFP__GUI__BROWSER_WINDOW_H
