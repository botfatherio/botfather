#include "browser_window.h"
#include "ui_browser_window.h"
#include <QDebug>
#include <QStyle>
#include <QScreen>
#include <QSettings>
#include "../../engine/modules/browser/browser_settings.h"

BrowserWindow::BrowserWindow(const QString &title, QWidget *parent)
	: QMainWindow(parent)
	, m_ui(new Ui::BrowserWindow)
{
	m_ui->setupUi(this);
	m_browser_widget = new BrowserWidget(this);
	m_adressbar = new BrowserAddressBar(this);
	m_ui->centralwidget->layout()->addWidget(m_browser_widget);
	m_ui->toolBar->addWidget(m_adressbar);

	layout()->setSizeConstraint(QLayout::SetFixedSize);
	setWindowTitle(title);

	connect(this, &BrowserWindow::windowMoved, m_browser_widget, &BrowserWidget::emitGlobalPosition);
	connect(this, &BrowserWindow::windowActivated, m_browser_widget, &BrowserWidget::emitGlobalPosition);

	connect(m_ui->actionHome, &QAction::triggered, this, &BrowserWindow::homeTriggered);
	connect(m_ui->actionReload, &QAction::triggered, this, &BrowserWindow::reloadTriggered);
	connect(m_ui->actionStop, &QAction::triggered, this, &BrowserWindow::stopTriggered);
	connect(m_ui->actionBack, &QAction::triggered, this, &BrowserWindow::backTriggered);
	connect(m_ui->actionForward, &QAction::triggered, this, &BrowserWindow::forwardTriggered);
}

BrowserWindow::~BrowserWindow()
{
	delete m_ui;
}

BrowserWidget* BrowserWindow::browserWidget() const
{
	return m_browser_widget;
}

BrowserAddressBar* BrowserWindow::addressBar() const
{
	return m_adressbar;
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

void BrowserWindow::updateNavigationButtons(bool is_loading, bool can_go_back, bool can_go_forward)
{
	m_ui->actionBack->setEnabled(can_go_back);
	m_ui->actionForward->setEnabled(can_go_forward);
	m_ui->actionReload->setVisible(!is_loading);
	m_ui->actionStop->setVisible(is_loading);
}

bool BrowserWindow::event(QEvent *e)
{
	switch (e->type()) {
	case QEvent::WindowActivate:
		emit windowActivated();
		break;
	case QEvent::Move:
		emit windowMoved();
	default:
		break;
	}
	return QMainWindow::event(e);
}
