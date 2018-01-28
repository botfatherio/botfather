#ifndef BROWSER_API_H
#define BROWSER_API_H

#include <QObject>

class BrowserAPI : public QObject
{
	Q_OBJECT
public:
	explicit BrowserAPI(QObject *parent = nullptr);
	
signals:
	
public slots:
};

#endif // BROWSER_API_H