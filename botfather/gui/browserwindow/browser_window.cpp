#include "browser_window.h"
#include "ui_browser_window.h"
#include <QDebug>
#include <QStyle>
#include <QScreen>
#include <QSettings>
#include "../../engine/modules/browser/browser_settings.h"

BrowserWindow::BrowserWindow(QWidget *parent)
	: QMainWindow(parent)
	, m_ui(new Ui::BrowserWindow)
{
	m_ui->setupUi(this);
	m_browser_widget = new BrowserWidget(this);
	m_adressbar = new BrowserAddressBar(this);
	m_ui->centralwidget->layout()->addWidget(m_browser_widget);
	m_ui->toolBar->addWidget(m_adressbar);
}

BrowserWindow::~BrowserWindow()
{
	delete m_ui;
}

BrowserWidget* BrowserWindow::browserWidget() const
{
	return m_browser_widget;
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
	m_browser_widget->setPixmap(QPixmap::fromImage(browser_image));
}
