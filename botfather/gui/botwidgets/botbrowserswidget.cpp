#include "botbrowserswidget.h"
#include "ui_botbrowserswidget.h"
#include "../../engine/modules/browser/browser_host.h"

BotBrowsersWidget::BotBrowsersWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::BotBrowsersWidget)
{
	ui->setupUi(this);
	ui->browser_list_view->setModel(BrowserHost::instance()->model());
}

BotBrowsersWidget::~BotBrowsersWidget()
{
	delete ui;
}
