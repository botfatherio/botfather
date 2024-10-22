#include "browser_widget.hpp"

#include <QDebug>
#include <QMouseEvent>

BrowserWidget::BrowserWidget(QWidget *parent) : QLabel(parent) {
    // Attract mouse and keyboard events.
    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);
}

void BrowserWidget::emitGlobalPosition() {
    // Emit the the global coordinates of the top-left pixel of the
    // BrowserWidget.
    emit globalPositionChanged(mapToGlobal(QPoint(0, 0)));
}

void BrowserWidget::mousePressEvent(QMouseEvent *event) {
    emit mousePressed(event->pos(), event->button());
}

void BrowserWidget::mouseReleaseEvent(QMouseEvent *event) {
    emit mouseReleased(event->pos(), event->button());
}

void BrowserWidget::mouseMoveEvent(QMouseEvent *event) {
    emit mouseMoved(event->pos());
}

void BrowserWidget::wheelEvent(QWheelEvent *event) {
    emit wheelScrolled(event->position().toPoint(), event->angleDelta());
}

void BrowserWidget::keyPressEvent(QKeyEvent *event) { emit keyPressed(event); }

void BrowserWidget::keyReleaseEvent(QKeyEvent *event) {
    emit keyReleased(event);
}
