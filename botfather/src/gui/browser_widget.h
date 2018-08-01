#ifndef BFP__GUI__BROWSER_WIDGET_H
#define BFP__GUI__BROWSER_WIDGET_H

#include <QLabel>
#include <QImage>

class BrowserWidget : public QLabel {
	Q_OBJECT

public:
	explicit BrowserWidget(QWidget* parent = nullptr);

private:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent* event);
	void keyPressEvent(QKeyEvent * event);
	void keyReleaseEvent(QKeyEvent * event);
};

#endif // BFP__GUI__BROWSER_WIDGET_H
