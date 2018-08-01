#ifndef BFP__GUI__BROWSER_WINDOW_H
#define BFP__GUI__BROWSER_WINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QImage>
#include <QTimer>

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
	void applyResizing();
	void filterOldSize(QImage browser_image);
	void paintSlot(QImage browser_image);
	void on_actionHome_triggered();
	void updateNavigationButtons(bool browser_loading_state);
	
private:
	Ui::BrowserWindow *ui;
	BrowserWidget* pixmap_placeholder;
	BrowserAddressBar* address_bar;
	QLineEdit* addressbar;
	QTimer resize_timer;
	void showEvent(QShowEvent * event);
	void hideEvent(QHideEvent *event);
	void resizeEvent(QResizeEvent *event);
};

#endif // BFP__GUI__BROWSER_WINDOW_H
