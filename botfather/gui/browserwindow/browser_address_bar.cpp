#include "browser_address_bar.hpp"

BrowserAddressBar::BrowserAddressBar(QWidget* parent) : QLineEdit(parent) {
    // Max URL length.
    // https://stackoverflow.com/questions/417142/what-is-the-maximum-length-of-a-url-in-different-browsers
    setMaxLength(2000);
    setClearButtonEnabled(true);
    connect(this, SIGNAL(returnPressed()), SLOT(navigate()));
}

void BrowserAddressBar::navigate() {
    // CEF does a good job in interpreting texts as urls. No need to help out.
    emit urlEntered(text());
}
