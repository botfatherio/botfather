#include "browser_widget.h"
#include <QMouseEvent>
#include "../browser/browser_client.h"
#include "../browser/browser.h"

BrowserWidget::BrowserWidget(QWidget* parent)
{
	Q_UNUSED(parent);
	
	// The browser widget must stay in it's desired size, otherwise mouseinput has wrong coords.
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	
	// Attract mouse and keyboard events.
	this->setMouseTracking(true);
	this->setFocusPolicy(Qt::ClickFocus);
	
	// Auto connect own paint slot to the browsers paint signal.
	QObject::connect(BrowserClient::instance(), SIGNAL(paintSignal(QImage)), this, SLOT(paintSlot(QImage)));
}

void BrowserWidget::paintSlot(QImage browser_image)
{
	this->setPixmap(QPixmap::fromImage(browser_image));
}

void BrowserWidget::mousePressEvent(QMouseEvent *event)
{
	int cef_button_code = Browser::qtToCefMouseButtonType(event->button());
	if (cef_button_code == -1)
		return;
	Browser::pressMouse(cef_button_code, event->x(), event->y());
}

void BrowserWidget::mouseReleaseEvent(QMouseEvent *event)
{
	int cef_button_code = Browser::qtToCefMouseButtonType(event->button());
	if (cef_button_code == -1)
		return;
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

void BrowserWidget::keyPressEvent(QKeyEvent * event)
{}

void BrowserWidget::keyReleaseEvent(QKeyEvent * event)
{}