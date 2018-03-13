#include "browser_widget.h"
#include <QMouseEvent>
#include <QMessageBox>
#include "../browser/browser_client.h"
#include "../browser/browser.h"

BrowserWidget::BrowserWidget(QWidget* parent)
{
	Q_UNUSED(parent);
	
	// Make this widget expand to it's parents size.
	this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	
	// Make this widgets content (like the resizing text) be centered.
	this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	
	// Attract mouse and keyboard events.
	this->setMouseTracking(true);
	this->setFocusPolicy(Qt::ClickFocus);
}

void BrowserWidget::mousePressEvent(QMouseEvent *event)
{
	int cef_button_code = Browser::qtToCefMouseButtonType(event->button());
	if (cef_button_code == -1) {
		return;
	}
	Browser::pressMouse(cef_button_code, event->x(), event->y());
}

void BrowserWidget::mouseReleaseEvent(QMouseEvent *event)
{
	int cef_button_code = Browser::qtToCefMouseButtonType(event->button());
	if (cef_button_code == -1) {
		return;
	}
	Browser::releaseMouse(cef_button_code, event->x(), event->y());
}

void BrowserWidget::mouseMoveEvent(QMouseEvent *event)
{
	Browser::moveMouse(event->x(), event->y());
}

void BrowserWidget::wheelEvent(QWheelEvent *event)
{
	Browser::scrollWheel(event->x(), event->y(), event->angleDelta().x(), event->angleDelta().y());
}

void BrowserWidget::keyPressEvent(QKeyEvent *event)
{
	QMessageBox::information(this, "No Keyboard Support yet!", "Use the rightclick context menu to paste text into fields on the website.");
}

void BrowserWidget::keyReleaseEvent(QKeyEvent *event)
{}