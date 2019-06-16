#include "browser_client.hpp"

BrowserClient::BrowserClient(const QSize &size)
	: QObject()
	, BrowserLifeSpanHandler()
	, BrowserRenderHandler(this, size)
	, BrowserLoadHandler(this)
	, BrowserRequestHandler(this)
{

}

void BrowserClient::setScreenPointOffset(const QPoint &offset)
{
	BrowserRenderHandler::setScreenPointOffset(offset);
}
