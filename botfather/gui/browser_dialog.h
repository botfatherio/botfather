#ifndef BROWSER_DIALOG_H
#define BROWSER_DIALOG_H

#include <QDialog>
#include <QImage>
#include <QTimer>

namespace Ui {
class BrowserDialog;
}

class BrowserDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit BrowserDialog(QWidget *parent = 0);
	~BrowserDialog();

public slots:
	void applyResizing();
	void filterOldSize(QImage browser_image);
	void paintSlot(QImage browser_image);
	
private:
	Ui::BrowserDialog *ui;
	QTimer resize_timer;
	void showEvent(QShowEvent * event);
	void hideEvent(QHideEvent *event);
	void resizeEvent(QResizeEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent* event);
	void keyPressEvent(QKeyEvent * event);
	void keyReleaseEvent(QKeyEvent * event);
};

#endif // BROWSER_DIALOG_H
