#include "browser_widget.h"
#include <QMouseEvent>
#include <QMessageBox>
#include "../engine/modules/browser/browser_client.h"
#include "../engine/modules/browser/browser.h"

BrowserWidget::BrowserWidget(QWidget* parent) : QLabel(parent)
{
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
	int cef_button_code = Browser::qtToCefMouseButtonType(static_cast<int>(event->button()));
	if (cef_button_code == -1) {
		return;
	}
	Browser::pressMouse(cef_button_code, event->pos());
}

void BrowserWidget::mouseReleaseEvent(QMouseEvent *event)
{
	int cef_button_code = Browser::qtToCefMouseButtonType(static_cast<int>(event->button()));
	if (cef_button_code == -1) {
		return;
	}
	Browser::releaseMouse(cef_button_code, event->pos());
}

void BrowserWidget::mouseMoveEvent(QMouseEvent *event)
{
	Browser::moveMouse(event->pos());
}

void BrowserWidget::wheelEvent(QWheelEvent *event)
{
	Browser::scrollWheel(event->pos(), event->angleDelta().x(), event->angleDelta().y());
}

void BrowserWidget::keyPressEvent(QKeyEvent *event)
{
	if (event->matches(QKeySequence::Copy))
	{
		BrowserClient::instance()->getBrowser()->GetFocusedFrame()->Copy();
	}
	else if (event->matches(QKeySequence::Cut))
	{
		BrowserClient::instance()->getBrowser()->GetFocusedFrame()->Cut();
	}
	else if (event->matches(QKeySequence::Paste))
	{
		BrowserClient::instance()->getBrowser()->GetFocusedFrame()->Paste();
	}
	else if (event->matches(QKeySequence::Undo))
	{
		BrowserClient::instance()->getBrowser()->GetFocusedFrame()->Undo();
	}
	else if (event->matches(QKeySequence::Redo))
	{
		BrowserClient::instance()->getBrowser()->GetFocusedFrame()->Redo();
	}
	else if (event->matches(QKeySequence::SelectAll))
	{
		BrowserClient::instance()->getBrowser()->GetFocusedFrame()->SelectAll();
	}
}

void BrowserWidget::keyReleaseEvent(QKeyEvent *event)
{
	Q_UNUSED(event)
}
