#include "browser_api.h"
#include "browser.h"

BrowserAPI::BrowserAPI(QJSEngine* engine_p) : m_engine_p(engine_p)
{}

// static
void BrowserAPI::enable(QJSEngine *engine_p)
{
	QJSValue vision_obj = engine_p->newQObject(new BrowserAPI(engine_p));
	engine_p->globalObject().setProperty("Browser", vision_obj);
}

void BrowserAPI::blockRessource(QString ressource)
{
	Browser::blockRessource(ressource);
}

void BrowserAPI::replaceRessource(QString old_ressource, QString new_ressource)
{
	Browser::replaceRessource(old_ressource, new_ressource);
}

void BrowserAPI::unmodifyRessource(QString ressource)
{
	Browser::unmodifyRessource(ressource);
}

void BrowserAPI::unmodifyRessources()
{
	Browser::unmodifyRessources();
}

void BrowserAPI::loadUrl(QString url)
{
	Browser::loadUrl(url);
}

QString BrowserAPI::getUrl()
{
	return Browser::getUrl();
}

void BrowserAPI::reload()
{
	Browser::reload();
}

void BrowserAPI::reloadIgnoringCache()
{
	Browser::reloadIgnoringCache();
}

void BrowserAPI::stopLoad()
{
	Browser::stopLoad();
}

bool BrowserAPI::canGoBack()
{
	return Browser::canGoBack();
}

bool BrowserAPI::canGoForward()
{
	return Browser::canGoForward();
}

void BrowserAPI::goBack()
{
	Browser::goBack();
}

void BrowserAPI::goForward()
{
	Browser::goForward();
}

int BrowserAPI::getWidth()
{
	return Browser::getWidth();
}

int BrowserAPI::getHeight()
{
	return Browser::getHeight();
}

void BrowserAPI::executeJavascript(QString javascript_code)
{
	Browser::executeJavascript(javascript_code);
}

void BrowserAPI::leftClickAt(int x, int y)
{
	Browser::clickAt(MBT_LEFT, x, y);
}

void BrowserAPI::middleClickAt(int x, int y)
{
	Browser::clickAt(MBT_MIDDLE, x, y);
}

void BrowserAPI::rightClickAt(int x, int y)
{
	Browser::clickAt(MBT_RIGHT, x, y);
}

void BrowserAPI::warpMouseTo(int x, int y)
{
	Browser::warpMouseTo(x, y);
}

void BrowserAPI::scrollWheel(int x, int y, int delta_x, int delta_y)
{
	Browser::scrollWheel(x, y, delta_x, delta_y);
}