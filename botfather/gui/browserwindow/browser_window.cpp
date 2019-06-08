#include "browser_window.h"
#include "ui_browser_window.h"
#include <QDebug>
#include <QAction>
#include <QStyle>
#include <QScreen>
#include <QSettings>
#include "browser_widget.h"
#include "browser_address_bar.h"
#include "../../engine/modules/browser/browser_settings.h"

BrowserWindow::BrowserWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::BrowserWindow)
{
	ui->setupUi(this);
	pixmap_placeholder = new BrowserWidget(this);
	addressbar = new BrowserAddressBar(this);
	ui->centralwidget->layout()->addWidget(pixmap_placeholder);
	ui->toolBar->addWidget(addressbar);
}

BrowserWindow::~BrowserWindow()
{
	delete ui;
}

void BrowserWindow::show()
{
    // Center the browser window. On windows 10 the top bar won't be visible initialy otherwise
    static bool first_show = true;
    if (first_show)
    {
        first_show = false;
		setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qGuiApp->primaryScreen()->availableGeometry()));
    }
    QMainWindow::show();
}

void BrowserWindow::paintSlot(QImage browser_image)
{
	pixmap_placeholder->setPixmap(QPixmap::fromImage(browser_image));
}
