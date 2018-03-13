#include "browser_dialog.h"
#include "ui_browser_dialog.h"
#include <QMouseEvent>
#include <QSettings>
#include "../shared/constants.h"
#include "../browser/browser_client.h"
#include "../browser/browser.h"

BrowserDialog::BrowserDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::BrowserDialog)
{
	ui->setupUi(this);
	
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
	Browser::resize(event->size());
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