#ifndef BFP__GUI__BROWSER_WIDGET_H
#define BFP__GUI__BROWSER_WIDGET_H

#include <QLabel>

class BrowserWidget : public QLabel {
	Q_OBJECT

public:
	explicit BrowserWidget(QWidget* parent = nullptr);

signals:
	void mousePressed(const QPoint &position, int qt_mouse_button);
	void mouseReleased(const QPoint &position, int qt_mouse_button);
	void mouseMoved(const QPoint &position);
	void wheelScrolled(const QPoint &position, const QPoint &delta);
	void keyPressed(const QKeyEvent *event);
	void keyReleased(const QKeyEvent *event);

private:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
};

#endif // BFP__GUI__BROWSER_WIDGET_H
