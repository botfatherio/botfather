#ifndef BROWSERCREATOR_H
#define BROWSERCREATOR_H

#include <QObject>
#include <QSize>
#include "browser_client.h"

class BrowserCreator : public QObject
{
	Q_OBJECT
public:
	// Creates a CefBrowser synchronous in the applications main thread, no matter what thread this
	// method is called from.
	static CefRefPtr<CefBrowser> createBrowserSync(const QSize &size);

private:
	explicit BrowserCreator(const QSize &size, QObject *parent = nullptr);
	CefRefPtr<CefBrowser> browser() const;

private slots:
	void process();

signals:
	void finished();

private:
	QSize m_browser_size;
	QString m_browser_id;
	CefRefPtr<CefBrowser> m_browser;
};

#endif // BROWSERCREATOR_H
