#include "botbrowserswidget.h"
#include "ui_botbrowserswidget.h"
#include <QDebug>
#include "../../engine/modules/browser/browser_manager.h"
#include "../browserwindow/browser_window.h"

BotBrowsersWidget::BotBrowsersWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::BotBrowsersWidget)
{
	ui->setupUi(this);
	ui->browser_list_view->setModel(BrowserManager::instance()->model());
	connect(ui->browser_list_view, &QTableView::doubleClicked, this, &BotBrowsersWidget::viewBrowser);
}

BotBrowsersWidget::~BotBrowsersWidget()
{
	delete ui;
}

void BotBrowsersWidget::viewBrowser(const QModelIndex &index)
{
	Browser *browser = qvariant_cast<Browser*>(BrowserManager::instance()->model()->data(index, BrowserModel::BROWSER_PTR_ROLE));
	if (!browser) return;

	BrowserClient *browser_client = browser->client();
	if (!browser_client) return;

	BrowserWindow *browser_window = new BrowserWindow(this);

	connect(browser->client(), &BrowserClient::paintSignal, browser_window, &BrowserWindow::paintSlot);
	connect(browser->client(), &BrowserClient::finishedLoadingUrl, browser_window->addressBar(), &BrowserAddressBar::setText);

	// For some reasons these signals get never received if queued connections are used.
	connect(browser_window->addressBar(), &BrowserAddressBar::urlEntered, browser, &Browser::loadUrl, Qt::DirectConnection);
	connect(browser_window->browserWidget(), &BrowserWidget::mousePressed, browser, &Browser::pressMouse, Qt::DirectConnection);
	connect(browser_window->browserWidget(), &BrowserWidget::mouseReleased, browser, &Browser::releaseMouse, Qt::DirectConnection);
	connect(browser_window->browserWidget(), &BrowserWidget::mouseMoved, browser, &Browser::moveMouse, Qt::DirectConnection);
	connect(browser_window->browserWidget(), &BrowserWidget::wheelScrolled, browser, &Browser::scrollWheel, Qt::DirectConnection);
	connect(browser_window->browserWidget(), &BrowserWidget::keyPressed, browser, &Browser::pressKey, Qt::DirectConnection);
	connect(browser_window->browserWidget(), &BrowserWidget::keyReleased, browser, &Browser::releaseKey, Qt::DirectConnection);

	browser_window->show();

	// When creating and opening a new BrowserWindow it won't display anything until the BrowserClients OnPaint method sends a new image.
	// But the webpage is only re-rendered when something changed. To force re-rendering of the webpage we invalidate the browsers view.
	browser->cefBrowser()->GetHost()->Invalidate(PET_VIEW);
}
