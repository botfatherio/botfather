#include "botbrowserswidget.h"
#include "ui_botbrowserswidget.h"
#include "../../engine/modules/browser/browser_manager.h"

BotBrowsersWidget::BotBrowsersWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::BotBrowsersWidget)
{
	ui->setupUi(this);
	ui->browser_list_view->setModel(BrowserManager::instance()->model());
}

BotBrowsersWidget::~BotBrowsersWidget()
{
	delete ui;
}
