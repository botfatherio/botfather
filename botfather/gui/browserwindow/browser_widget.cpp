#include "browser_widget.h"
#include <QMouseEvent>
#include <QMessageBox>

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
	/*
	int cef_button_code = OldBrowser::qtToCefMouseButtonType(static_cast<int>(event->button()));
	if (cef_button_code == -1) {
		return;
	}
	OldBrowser::pressMouse(cef_button_code, event->pos());
	*/
}

void BrowserWidget::mouseReleaseEvent(QMouseEvent *event)
{
	/*
	int cef_button_code = OldBrowser::qtToCefMouseButtonType(static_cast<int>(event->button()));
	if (cef_button_code == -1) {
		return;
	}
	OldBrowser::releaseMouse(cef_button_code, event->pos());
	*/
}

void BrowserWidget::mouseMoveEvent(QMouseEvent *event)
{
	//OldBrowser::moveMouse(event->pos());
}

void BrowserWidget::wheelEvent(QWheelEvent *event)
{
	//OldBrowser::scrollWheel(event->pos(), event->angleDelta().x(), event->angleDelta().y());
}

void BrowserWidget::keyPressEvent(QKeyEvent *event)
{
	/*
	if (event->matches(QKeySequence::Copy))
	{
		OldBrowserClient::instance()->getBrowser()->GetFocusedFrame()->Copy();
	}
	else if (event->matches(QKeySequence::Cut))
	{
		OldBrowserClient::instance()->getBrowser()->GetFocusedFrame()->Cut();
	}
	else if (event->matches(QKeySequence::Paste))
	{
		OldBrowserClient::instance()->getBrowser()->GetFocusedFrame()->Paste();
	}
	else if (event->matches(QKeySequence::Undo))
	{
		OldBrowserClient::instance()->getBrowser()->GetFocusedFrame()->Undo();
	}
	else if (event->matches(QKeySequence::Redo))
	{
		OldBrowserClient::instance()->getBrowser()->GetFocusedFrame()->Redo();
	}
	else if (event->matches(QKeySequence::SelectAll))
	{
		OldBrowserClient::instance()->getBrowser()->GetFocusedFrame()->SelectAll();
	}
	*/
}

void BrowserWidget::keyReleaseEvent(QKeyEvent *event)
{
	Q_UNUSED(event)
}
