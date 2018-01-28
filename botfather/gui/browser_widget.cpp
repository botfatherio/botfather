#include "browser_widget.h"
#include <QMouseEvent>
#include "browser/browser_client.h"

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
	CefMouseEvent cms;
	cms.x = event->x();
	cms.y = event->y();
	if (event->button() == Qt::LeftButton)
		BrowserClient::instance()->getBrowser()->GetHost()->SendMouseClickEvent(cms, MBT_LEFT, false, 1);
	else if (event->button() == Qt::RightButton)
		BrowserClient::instance()->getBrowser()->GetHost()->SendMouseClickEvent(cms, MBT_RIGHT, false, 1);
}

void BrowserWidget::mouseReleaseEvent(QMouseEvent *event)
{
	CefMouseEvent cms;
	cms.x = event->x();
	cms.y = event->y();
	if (event->button() == Qt::LeftButton)
		BrowserClient::instance()->getBrowser()->GetHost()->SendMouseClickEvent(cms, MBT_LEFT, true, 1);
	else if (event->button() == Qt::RightButton)
		BrowserClient::instance()->getBrowser()->GetHost()->SendMouseClickEvent(cms, MBT_RIGHT, true, 1);
}

void BrowserWidget::mouseMoveEvent(QMouseEvent *event)
{
	CefMouseEvent cme;
	cme.x = event->x();
	cme.y = event->y();
	bool mouse_leave = event->x() < 0 || event->y() < 0;
	BrowserClient::instance()->getBrowser()->GetHost()->SendMouseMoveEvent(cme, mouse_leave);
}

void BrowserWidget::wheelEvent(QWheelEvent *event)
{
	CefMouseEvent cme;
	cme.x = event->x();
	cme.y = event->y();
	BrowserClient::instance()->getBrowser()->GetHost()->SendMouseWheelEvent(
		cme,
		event->angleDelta().x(),
		event->angleDelta().y()
	);
}

void BrowserWidget::keyPressEvent(QKeyEvent * event)
{
	
}

void BrowserWidget::keyReleaseEvent(QKeyEvent * event)
{
	
}