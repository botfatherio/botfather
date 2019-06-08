#ifndef BFP__GUI__BROWSER_ADDRESS_BAR_H
#define BFP__GUI__BROWSER_ADDRESS_BAR_H

#include <QLineEdit>

class BrowserAddressBar : public QLineEdit
{
	Q_OBJECT
	
public:
	explicit BrowserAddressBar(QWidget* parent = nullptr);
	
public slots:
	void navigate();

signals:
	void urlEntered(const QString &url);
};

#endif // BFP__GUI__BROWSER_ADDRESS_BAR_H
