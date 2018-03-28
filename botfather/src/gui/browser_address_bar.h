#ifndef BFP__GUI__BROWSER_ADDRESS_BAR_H
#define BFP__GUI__BROWSER_ADDRESS_BAR_H

#include <QLineEdit>

class BrowserAddressBar : public QLineEdit
{
	Q_OBJECT
	
public:
	BrowserAddressBar(QWidget* parent = 0);
	
public slots:
	void navigate();
	void updateUrl();
};

#endif // BFP__GUI__BROWSER_ADDRESS_BAR_H