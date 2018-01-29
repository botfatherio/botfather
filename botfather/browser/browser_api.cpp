#include "browser_api.h"
#include "browser_client.h"
#include "../shared/constants.h"

BrowserAPI::BrowserAPI(QJSEngine* engine_p) : m_engine_p(engine_p)
{}

void BrowserAPI::enable(QJSEngine *engine_p)
{
	QJSValue vision_obj = engine_p->newQObject(new BrowserAPI(engine_p));
	engine_p->globalObject().setProperty("Browser", vision_obj);
}

void BrowserAPI::blockRessource(QString ressource)
{
	BrowserClient::instance()->blockRessource(ressource);
}

void BrowserAPI::replaceRessource(QString old_ressource, QString new_ressource)
{
	BrowserClient::instance()->replaceRessource(old_ressource, new_ressource);
}

void BrowserAPI::unmodifyRessource(QString ressource)
{
	BrowserClient::instance()->unmodifyRessource(ressource);
}

void BrowserAPI::unmodifyRessources()
{
	BrowserClient::instance()->unmodifyRessources();
}

void BrowserAPI::loadUrl(QString url)
{
	BrowserClient::instance()->getBrowser()->GetMainFrame()->LoadURL(url.toStdString());
}

QString BrowserAPI::getUrl()
{
	return QString::fromStdString(BrowserClient::instance()->getBrowser()->GetMainFrame()->GetURL().ToString());
}

void BrowserAPI::reload()
{
	BrowserClient::instance()->getBrowser()->Reload();
}

void BrowserAPI::reloadIgnoreCache()
{
	BrowserClient::instance()->getBrowser()->ReloadIgnoreCache();
}

void BrowserAPI::stopLoad()
{
	BrowserClient::instance()->getBrowser()->StopLoad();
}

bool BrowserAPI::canGoBack()
{
	return BrowserClient::instance()->getBrowser()->CanGoBack();
}

bool BrowserAPI::canGoForward()
{
	return BrowserClient::instance()->getBrowser()->CanGoForward();
}

void BrowserAPI::goBack()
{
	BrowserClient::instance()->getBrowser()->GoBack();
}

void BrowserAPI::goForward()
{
	BrowserClient::instance()->getBrowser()->GoForward();
}

int BrowserAPI::getWidth()
{
	return m_settings.value("BROWSER_WIDTH", constants::BROWSER_WIDTH).toInt();
}

int BrowserAPI::getHeight()
{
	return m_settings.value("BROWSER_HEIGHT", constants::BROWSER_HEIGHT).toInt();
}

void BrowserAPI::executeJavascript(QString javascript_code)
{
	BrowserClient::instance()->getBrowser()->GetMainFrame()->ExecuteJavaScript(
		javascript_code.toStdString(),
		BrowserClient::instance()->getBrowser()->GetMainFrame()->GetURL(),
		0
	);
}