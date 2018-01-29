#include "browser_api.h"
#include "browser_client.h"
#include "../shared/constants.h"

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
	BrowserClient::GetInstance()->blockRessource(ressource);
}

void BrowserAPI::replaceRessource(QString old_ressource, QString new_ressource)
{
	BrowserClient::GetInstance()->replaceRessource(old_ressource, new_ressource);
}

void BrowserAPI::unmodifyRessource(QString ressource)
{
	BrowserClient::GetInstance()->unmodifyRessource(ressource);
}

void BrowserAPI::unmodifyRessources()
{
	BrowserClient::GetInstance()->unmodifyRessources();
}

void BrowserAPI::loadUrl(QString url)
{
	BrowserClient::GetInstance()->GetBrowser()->GetMainFrame()->LoadURL(url.toStdString());
}

QString BrowserAPI::getUrl()
{
	return QString::fromStdString(BrowserClient::GetInstance()->GetBrowser()->GetMainFrame()->GetURL().ToString());
}

void BrowserAPI::reload()
{
	BrowserClient::GetInstance()->GetBrowser()->Reload();
}

void BrowserAPI::reloadIgnoreCache()
{
	BrowserClient::GetInstance()->GetBrowser()->ReloadIgnoreCache();
}

void BrowserAPI::stopLoad()
{
	BrowserClient::GetInstance()->GetBrowser()->StopLoad();
}

bool BrowserAPI::canGoBack()
{
	return BrowserClient::GetInstance()->GetBrowser()->CanGoBack();
}

bool BrowserAPI::canGoForward()
{
	return BrowserClient::GetInstance()->GetBrowser()->CanGoForward();
}

void BrowserAPI::goBack()
{
	BrowserClient::GetInstance()->GetBrowser()->GoBack();
}

void BrowserAPI::goForward()
{
	BrowserClient::GetInstance()->GetBrowser()->GoForward();
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
	BrowserClient::GetInstance()->GetBrowser()->GetMainFrame()->ExecuteJavaScript(
		javascript_code.toStdString(),
		BrowserClient::GetInstance()->GetBrowser()->GetMainFrame()->GetURL(),
		0
	);
}