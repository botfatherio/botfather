#include "browser_dialog.h"
#include "ui_browser_dialog.h"
#include <QMouseEvent>
#include <QSettings>
#include <QMessageBox>
#include "../shared/constants.h"
#include "../browser/browser_client.h"
#include "../browser/browser.h"

BrowserDialog::BrowserDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::BrowserDialog)
{
	ui->setupUi(this);
	
	// Setup a singleshot timer used to resize the browser once the user stopped
	// resizing the whole window.
	resize_timer.setSingleShot(true);
	connect(&resize_timer, SIGNAL(timeout()), SLOT(applyResizing()));
	
	// Setup a min and max size for the window and the pixmap placeholder
	this->setMinimumSize(512, 256);
	this->setMaximumSize(4096, 2160);
	
	// Remember and set the browser dialogs geometry.
	QSettings settings;
	int width = settings.value("BROWSER_WIDTH", constants::BROWSER_WIDTH).toInt();
	int height = settings.value("BROWSER_HEIGHT", constants::BROWSER_HEIGHT).toInt();
	this->setGeometry(0, 0, width, height);
}

BrowserDialog::~BrowserDialog()
{
	delete ui;
}

void BrowserDialog::applyResizing()
{
	// The user stopped resizing the window. Resize the browser accordingly.
	int width = this->ui->pixmap_placeholder->width();
	int height = this->ui->pixmap_placeholder->height();
	Browser::resize(QSize(width, height));
	
	// Redirect the first few paint event signals to a slot filtering all browser
	// images which are still of the windows old size to prevent the window from
	// going back to it's old size due to the pixmap being potentially as large as
	// the old window size was. Which may happen due to the browsers resize method
	// being async.
	connect(BrowserClient::instance(), SIGNAL(paintSignal(QImage)), this, SLOT(filterOldSize(QImage)));
}

void BrowserDialog::filterOldSize(QImage browser_image)
{
	if (browser_image.size() == ui->pixmap_placeholder->size()) {
		// The browser now sends images of the placeholders size. Disconnect the filter
		// redirect the paint signals to the normal paint slot.
		disconnect(BrowserClient::instance(), SIGNAL(paintSignal(QImage)), this, SLOT(filterOldSize(QImage)));
		connect(BrowserClient::instance(), SIGNAL(paintSignal(QImage)), this, SLOT(paintSlot(QImage)));
	}
}

void BrowserDialog::paintSlot(QImage browser_image)
{
	this->ui->pixmap_placeholder->setPixmap(QPixmap::fromImage(browser_image));
}

void BrowserDialog::showEvent(QShowEvent *event)
{
	// Attract mouse and keyboard events.
	this->setMouseTracking(true);
	this->setFocusPolicy(Qt::ClickFocus);
	
	// Start rendering the website content to the window when it's shown.
	QObject::connect(BrowserClient::instance(), SIGNAL(paintSignal(QImage)), this, SLOT(paintSlot(QImage)));
}

void BrowserDialog::hideEvent(QHideEvent *event)
{
	// Stop rendering the websites content when the browser dialog gets closed.
	QObject::disconnect(BrowserClient::instance(), SIGNAL(paintSignal(QImage)), this, SLOT(paintSlot(QImage)));
}

void BrowserDialog::resizeEvent(QResizeEvent *event)
{
	// No longer paint stuff before the user decided what size he wants.
	// We do no longer paint because qt tends to do segmentation faults.
	// Thats due to qt and cef being async and having seperate event loops.
	
	// Save ressources by only clearing the pixmap and settings the info text once.
	if (disconnect(BrowserClient::instance(), SIGNAL(paintSignal(QImage)), this, SLOT(paintSlot(QImage)))){
		
		// Remove the last painted pixmap to enable the user to freely resize the
		// browser dialog. Not clearing the pixmap makes it impossible to shrink
		// the dialog, because the pixmap being as large as the window is.
		this->ui->pixmap_placeholder->clear();
		
		// Show a 'Resizing...' message while resizing. We do so because the user
		// might otherwise wonder why the pixmap is no longer displayed.
		this->ui->pixmap_placeholder->setText(
			"<p style='font-size:32px;font-weight:700;color:#333;'>Resizing...</p>"
		);
	}
	
	// This timer will timeout when the user finally stops the resize the window.
	// We do this to not have to process every single resize event on every pixel change.
	resize_timer.start(500);
}

void BrowserDialog::mousePressEvent(QMouseEvent *event)
{
	int cef_button_code = Browser::qtToCefMouseButtonType(event->button());
	if (cef_button_code == -1) {
		return;
	}
	Browser::pressMouse(cef_button_code, event->x(), event->y());
}

void BrowserDialog::mouseReleaseEvent(QMouseEvent *event)
{
	int cef_button_code = Browser::qtToCefMouseButtonType(event->button());
	if (cef_button_code == -1) {
		return;
	}
	Browser::releaseMouse(cef_button_code, event->x(), event->y());
}

void BrowserDialog::mouseMoveEvent(QMouseEvent *event)
{
	Browser::moveMouse(event->x(), event->y());
}

void BrowserDialog::wheelEvent(QWheelEvent *event)
{
	Browser::scrollWheel(event->x(), event->y(), event->angleDelta().x(), event->angleDelta().y());
}

void BrowserDialog::keyPressEvent(QKeyEvent * event)
{}

void BrowserDialog::keyReleaseEvent(QKeyEvent * event)
{}