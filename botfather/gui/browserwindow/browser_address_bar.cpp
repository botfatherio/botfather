#include "browser_address_bar.h"

BrowserAddressBar::BrowserAddressBar(QWidget* parent) : QLineEdit(parent)
{
	//setClearButtonEnabled(true);
	//setMaxLength(2000); // Url max length
	//connect(this, SIGNAL(returnPressed()), SLOT(navigate()));
	//connect(OldBrowserClient::instance(), SIGNAL(loadingFinishedSignal()), this, SLOT(updateUrl()));
}

void BrowserAddressBar::navigate()
{
	// CEF does a good job in interpreting text as urls. No need to help out.
	//OldBrowser::loadUrl(this->text());
}

void BrowserAddressBar::updateUrl()
{
	//setText(OldBrowser::getUrl().toString());
}
