#include "bot_browsers_widget.hpp"
#include "ui_bot_browsers_widget.h"
#include <QDebug>
#include "../../engine/modules/browser/browser_manager.hpp"
#include "../browserwindow/browser_window.hpp"

BotBrowsersWidget::BotBrowsersWidget(Bot *bot, QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::BotBrowsersWidget)
	, m_bot(bot)
	, m_browser_model_proxy(new QSortFilterProxyModel(this))
{
	ui->setupUi(this);

	m_browser_model_proxy->setSourceModel(BrowserManager::instance()->model());
	m_browser_model_proxy->setFilterRole(Qt::DisplayRole);
	m_browser_model_proxy->setFilterKeyColumn(0); // Group
	m_browser_model_proxy->setFilterFixedString(bot->scriptPath()); // The bots script path is used as its id

	ui->browser_list_view->setModel(m_browser_model_proxy);
	ui->browser_list_view->hideColumn(0); // Group

	connect(bot, &Bot::started, this, &BotBrowsersWidget::updateButtonStates);
	connect(bot, &Bot::stopped, this, &BotBrowsersWidget::updateButtonStates);
	connect(ui->browser_list_view->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &BotBrowsersWidget::updateButtonStates);

	connect(ui->browser_list_view, &QTableView::doubleClicked, this, &BotBrowsersWidget::viewBrowser);
	connect(ui->open_button, &QPushButton::clicked, this, &BotBrowsersWidget::viewCurrentBrowser);
	connect(ui->delete_button, &QPushButton::clicked, this, &BotBrowsersWidget::deleteCurrentBrowser);
}

BotBrowsersWidget::~BotBrowsersWidget()
{
	delete ui;
}

void BotBrowsersWidget::updateButtonStates()
{
	QModelIndex current = ui->browser_list_view->selectionModel()->currentIndex();
	ui->open_button->setEnabled(current.isValid());
	ui->delete_button->setEnabled(current.isValid() && !m_bot->isRunning());
}

void BotBrowsersWidget::deleteCurrentBrowser()
{
	deleteBrowser(ui->browser_list_view->selectionModel()->currentIndex());
}

void BotBrowsersWidget::deleteBrowser(const QModelIndex &index)
{
	if (!index.isValid() || m_bot->isRunning())
	{
		return;
	}

	Browser *browser = qvariant_cast<Browser*>(m_browser_model_proxy->data(index, BrowserListModel::BROWSER_PTR_ROLE));
	if (!browser) return;

	// Allow the browser to be closed by its BrowserClient
	browser->client()->setAllowedToClose(true);

	emit browser->aboutToGetDeletedByTheUser();
	BrowserManager::instance()->deleteBrowser(browser);
}

void BotBrowsersWidget::viewCurrentBrowser()
{
	viewBrowser(ui->browser_list_view->selectionModel()->currentIndex());
}

void BotBrowsersWidget::viewBrowser(const QModelIndex &index)
{
	if (!index.isValid())
	{
		return;
	}

	Browser *browser = qvariant_cast<Browser*>(m_browser_model_proxy->data(index, BrowserListModel::BROWSER_PTR_ROLE));
	if (!browser) return;

	BrowserClient *browser_client = browser->client();
	if (!browser_client) return;

	// Not giving the BrowserWindow the MainWindow as parent allows the user to put the MainWindow
	// in front of the BrowserWindow under Windows.
	// Not setting a parent puts us in charge of deleting the BrowserWindows. We do so using a signal.
	// Another problem is, that the BrowserWindows won't be closed when the MainWindow is closed.
	// Under linux giving the BrowserWindow a parent already had the desired behaviour.
	// Read MainWindow::closeEvent to learn how this issue was solved.
	BrowserWindow *browser_window = new BrowserWindow(browser->name(), nullptr);
	connect(this, &BotBrowsersWidget::destroyed, browser_window, &QMainWindow::deleteLater);

	connect(browser->client(), &BrowserClient::paintSignal, browser_window, &BrowserWindow::paintSlot);
	connect(browser->client(), &BrowserClient::finishedLoadingUrl, browser_window->addressBar(), &BrowserAddressBar::setText);
	connect(browser->client(), &BrowserClient::loadingStateChanged, browser_window, &BrowserWindow::updateNavigationButtons);

	// For some reasons these signals get never received if queued connections are used.
	connect(browser_window, &BrowserWindow::homeTriggered, browser, &Browser::goHome, Qt::DirectConnection);
	connect(browser_window, &BrowserWindow::reloadTriggered, browser, &Browser::reload, Qt::DirectConnection);
	connect(browser_window, &BrowserWindow::stopTriggered, browser, &Browser::stopLoading, Qt::DirectConnection);
	connect(browser_window, &BrowserWindow::backTriggered, browser, &Browser::goBack, Qt::DirectConnection);
	connect(browser_window, &BrowserWindow::forwardTriggered, browser, &Browser::goForward, Qt::DirectConnection);
	connect(browser_window->addressBar(), &BrowserAddressBar::urlEntered, browser, &Browser::loadUrl, Qt::DirectConnection);
	connect(browser_window->browserWidget(), &BrowserWidget::mousePressed, browser, &Browser::pressMouse, Qt::DirectConnection);
	connect(browser_window->browserWidget(), &BrowserWidget::mouseReleased, browser, &Browser::releaseMouse, Qt::DirectConnection);
	connect(browser_window->browserWidget(), &BrowserWidget::mouseMoved, browser, &Browser::moveMouse, Qt::DirectConnection);
	connect(browser_window->browserWidget(), &BrowserWidget::wheelScrolled, browser, &Browser::scrollWheel, Qt::DirectConnection);
	connect(browser_window->browserWidget(), &BrowserWidget::keyPressed, browser, QOverload<const QKeyEvent*>::of(&Browser::holdKey), Qt::DirectConnection);
	connect(browser_window->browserWidget(), &BrowserWidget::keyReleased, browser, QOverload<const QKeyEvent*>::of(&Browser::releaseKey), Qt::DirectConnection);

	// Close the BrowserWindow when the presented underlying Browser got deleted.
	// Beware: Using signals to close the BrowserWindow on application shutdown causes crashes.
	// Thats why we do not use the Browser::destroyed method, but a signal only emitted when the user
	// deletes the browser.
	connect(browser, &Browser::aboutToGetDeletedByTheUser, browser_window, &BrowserWindow::close);

	// When creating and opening a new BrowserWindow it won't display anything until the BrowserClients OnPaint method sends a new image.
	// But the webpage is only re-rendered when something changed. To force re-rendering of the webpage we invalidate the browsers view.
	browser->cefBrowser()->GetHost()->Invalidate(PET_VIEW);

	// The browser window navigation buttons and the url shown in the address bar are updated when certain signals got emitted.
	// To prevent an blank addressbar and not working navigation buttons, we call the related slots before showing the browserwindow.
	browser_window->addressBar()->setText(browser->url().toString());
	browser_window->updateNavigationButtons(browser->isLoading(), browser->canGoBack(), browser->canGoForward());

	// Inform the BrowserClient about the BrowserWidgets global position. This will ensure context menues are placed correctly.
	connect(browser_window->browserWidget(), &BrowserWidget::globalPositionChanged, browser_client, &BrowserClient::setScreenPointOffset);

	browser_window->show();
}
