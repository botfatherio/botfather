#include "browser_window.h"
#include "ui_browser_window.h"
#include <QDebug>
#include <QAction>
#include "browser_widget.h"
#include "browser_address_bar.h"
#include "../browser/browser_settings.h"
#include "../browser/browser_client.h"
#include "../browser/browser.h"

BrowserWindow::BrowserWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::BrowserWindow)
{
	ui->setupUi(this);
	
	// Setup a singleshot timer used to resize the browser once the user stopped
	// resizing the whole window.
	resize_timer.setSingleShot(true);
	connect(&resize_timer, SIGNAL(timeout()), SLOT(applyResizing()));
	
	// Setup a min and max size for the window and the pixmap placeholder
	this->setMinimumSize(512, 256);
	this->setMaximumSize(4096, 2160);
	
	// Add a modified label widget to this window which will take care of
	// mouse and keyboard events.
	pixmap_placeholder = new BrowserWidget(this);
	this->ui->centralwidget->layout()->addWidget(pixmap_placeholder);

	// Add a browser address bar to the browser window.
	addressbar = new BrowserAddressBar(this);
	ui->toolBar->addWidget(addressbar);

	// Remember and set the browser dialogs geometry.
	QSettings settings;
	int width = settings.value(browser::options::WIDTH, browser::fallback::WIDTH).toInt();
	int height = settings.value(browser::options::HEIGHT, browser::fallback::HEIGHT).toInt();
	int toolbar_height = this->ui->toolBar->height();
	this->setGeometry(0, 0, width, height + toolbar_height);
	
	// Connect browser window action singals with their respectiv offscreen browser slots.
	connect(ui->actionReload, &QAction::triggered, Browser::reload);
	connect(ui->actionStop, &QAction::triggered, Browser::stopLoad);
	connect(ui->actionBack, &QAction::triggered, Browser::goBack);
	connect(ui->actionForward, &QAction::triggered, Browser::goForward);
	
	// Update our navigation buttons every time the browser clients loading state changes.
	connect(BrowserClient::instance(), SIGNAL(loadingStateChangedSignal(bool)), this, SLOT(updateNavigationButtons(bool)));
}

BrowserWindow::~BrowserWindow()
{
	delete ui;
}

void BrowserWindow::applyResizing()
{
	// The user stopped resizing the window. Resize the browser accordingly.
	int width = this->pixmap_placeholder->width();
	int height = this->pixmap_placeholder->height();
	Browser::resize(QSize(width, height));
	
	// Redirect the first few paint event signals to a slot filtering all browser
	// images which are still of the windows old size to prevent the window from
	// going back to it's old size due to the pixmap being potentially as large as
	// the old window size was. Which may happen due to the browsers resize method
	// being async.
	connect(BrowserClient::instance(), SIGNAL(paintSignal(QImage)), this, SLOT(filterOldSize(QImage)));
}

void BrowserWindow::filterOldSize(QImage browser_image)
{
	if (browser_image.size() == this->pixmap_placeholder->size()) {
		
		// Render this first correct image to the window to make the "Resizing
		// Offscreen Browser" message disappear to not confuse the user.
		paintSlot(browser_image);
		
		// The browser now sends images of the placeholders size. Disconnect the filter
		// redirect the paint signals to the normal paint slot.
		disconnect(BrowserClient::instance(), SIGNAL(paintSignal(QImage)), this, SLOT(filterOldSize(QImage)));
		connect(BrowserClient::instance(), SIGNAL(paintSignal(QImage)), this, SLOT(paintSlot(QImage)));
	}
}

void BrowserWindow::paintSlot(QImage browser_image)
{
	this->pixmap_placeholder->setPixmap(QPixmap::fromImage(browser_image));
}

void BrowserWindow::on_actionHome_triggered()
{
	Browser::loadUrl("about:version");
}

void BrowserWindow::updateNavigationButtons(bool browser_loading_state)
{
	this->ui->actionBack->setEnabled(Browser::canGoBack());
	this->ui->actionForward->setEnabled(Browser::canGoForward());
	if (browser_loading_state) {
		ui->actionReload->setVisible(false);
		ui->actionStop->setVisible(true);
	} else {
		ui->actionReload->setVisible(true);
		ui->actionStop->setVisible(false);
	}
}

void BrowserWindow::showEvent(QShowEvent *event)
{
	Q_UNUSED(event)
	
	// Attract mouse and keyboard events.
	this->setMouseTracking(true);
	this->setFocusPolicy(Qt::ClickFocus);
	
	// Start rendering the website content to the window when it's shown.
	QObject::connect(BrowserClient::instance(), SIGNAL(paintSignal(QImage)), this, SLOT(paintSlot(QImage)));
}

void BrowserWindow::hideEvent(QHideEvent *event)
{
	Q_UNUSED(event)
	
	// Stop rendering the websites content when the browser dialog gets closed.
	QObject::disconnect(BrowserClient::instance(), SIGNAL(paintSignal(QImage)), this, SLOT(paintSlot(QImage)));
}

void BrowserWindow::resizeEvent(QResizeEvent *event)
{
	Q_UNUSED(event)
	
	// No longer paint stuff before the user decided what size he wants.
	// We do no longer paint because qt tends to do segmentation faults.
	// Thats due to qt and cef being async and having seperate event loops.
	
	// Save ressources by only clearing the pixmap and settings the info text once.
	if (disconnect(BrowserClient::instance(), SIGNAL(paintSignal(QImage)), this, SLOT(paintSlot(QImage)))){
		
		// Remove the last painted pixmap to enable the user to freely resize the
		// browser dialog. Not clearing the pixmap makes it impossible to shrink
		// the dialog, because the pixmap being as large as the window is.
		this->pixmap_placeholder->clear();
		
		// Show a 'Resizing...' message while resizing. We do so because the user
		// might otherwise wonder why the pixmap is no longer displayed.
		this->pixmap_placeholder->setText(
			"<p style='font-size:32px;font-weight:700;color:#333;'>Resizing Offscreen Browser...</p>"
		);
	}
	
	// This timer will timeout when the user finally stops the resize the window.
	// We do this to not have to process every single resize event on every pixel change.
	resize_timer.start(500);
}