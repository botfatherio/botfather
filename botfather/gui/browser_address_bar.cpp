#include "browser_address_bar.h"
#include "../browser/browser.h"
#include "../browser/browser_client.h"

BrowserAddressBar::BrowserAddressBar(QWidget* parent) : QLineEdit(parent)
{
	this->setClearButtonEnabled(true);
	this->setMaxLength(2000); // Url max length
	connect(this, SIGNAL(returnPressed()), SLOT(navigate()));
	connect(BrowserClient::instance(), SIGNAL(loadingFinishedSignal()), this, SLOT(updateUrl()));
}

void BrowserAddressBar::navigate()
{
	// CEF does a good job in interpreting text as urls. No need to help out.
	Browser::loadUrl(this->text());
}

void BrowserAddressBar::updateUrl()
{
	this->setText(Browser::getUrl());
}