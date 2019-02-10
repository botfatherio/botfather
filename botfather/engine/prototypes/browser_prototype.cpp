#include "browser_prototype.h"

// We must cast to Browser* otherwise the browsers pointers won't be valid.
#define THIS_BROWSER_P() qscriptvalue_cast<Browser*>(thisObject())

QScriptValue BrowserPrototype::constructor(QScriptContext *context, QScriptEngine *engine)
{
	// new Browser();
	if (context->argumentCount() == 0)
	{
		return engine->toScriptValue(Browser());
	}

	// new Browser(size);
	if (context->argumentCount() == 1 && isQSize(context->argument(0)))
	{
		QSize size(qscriptvalue_cast<QSize>(context->argument(0)));
		return engine->toScriptValue(Browser(size));
	}

	// new Browser(name);
	if (context->argumentCount() == 1 && context->argument(0).isString())
	{
		QString name(context->argument(0).toString());
		return engine->toScriptValue(Browser(name));
	}

	// new Browser(size, name);
	if (context->argumentCount() == 2 && isQSize(context->argument(0)) && context->argument(1).isString())
	{
		QSize size(qscriptvalue_cast<QSize>(context->argument(0)));
		QString name(context->argument(1).toString());
		return engine->toScriptValue(Browser(size, name));
	}

	NO_MATCHING_CTOR("Browser", BROWSER_PROTOTYPE_DOCS)
}

QImage BrowserPrototype::takeScreenshot()
{
	return THIS_BROWSER_P()->takeScreenshot();
}

QSize BrowserPrototype::getSize() const
{
	return THIS_BROWSER_P()->size();
}

QRect BrowserPrototype::getRect() const
{
	return THIS_BROWSER_P()->rect();
}

void BrowserPrototype::blockResource(const QString &resource_pattern)
{
	THIS_BROWSER_P()->blockResource(resource_pattern);
}

void BrowserPrototype::replaceResource(const QString &resource_pattern, const QString &replacement_url)
{
	THIS_BROWSER_P()->replaceResource(resource_pattern, replacement_url);
}

void BrowserPrototype::unmodifyResource(const QString &resource_pattern)
{
	THIS_BROWSER_P()->unmodifyResource(resource_pattern);
}

void BrowserPrototype::unmodifyResources()
{
	THIS_BROWSER_P()->unmodifyResources();
}

void BrowserPrototype::loadUrl(const QString &url)
{
	THIS_BROWSER_P()->loadUrl(url);
}

void BrowserPrototype::beOnUrl(const QString &url)
{
	THIS_BROWSER_P()->beOnUrl(url);
}

QUrl BrowserPrototype::getUrl() const
{
	return THIS_BROWSER_P()->url();
}

void BrowserPrototype::reload()
{
	THIS_BROWSER_P()->reload();
}

void BrowserPrototype::reloadIgnoringCache()
{
	THIS_BROWSER_P()->reloadIgnoringCache();
}

bool BrowserPrototype::isLoading() const
{
	return THIS_BROWSER_P()->isLoading();
}

bool BrowserPrototype::finishLoading(int timeout_in_seconds)
{
	return THIS_BROWSER_P()->finishLoading(timeout_in_seconds);
}

void BrowserPrototype::stopLoading()
{
	THIS_BROWSER_P()->stopLoading();
}

bool BrowserPrototype::canGoBack() const
{
	return THIS_BROWSER_P()->canGoBack();
}

bool BrowserPrototype::canGoForward() const
{
	return THIS_BROWSER_P()->canGoForward();
}

void BrowserPrototype::goForward()
{
	THIS_BROWSER_P()->goForward();
}

void BrowserPrototype::goBack()
{
	THIS_BROWSER_P()->goBack();
}

void BrowserPrototype::leftClick(const QPoint &position)
{
	THIS_BROWSER_P()->pressMouse(MBT_LEFT, position);
	THIS_BROWSER_P()->releaseMouse(MBT_LEFT, position);
}

void BrowserPrototype::middleClick(const QPoint &position)
{
	THIS_BROWSER_P()->pressMouse(MBT_MIDDLE, position);
	THIS_BROWSER_P()->releaseMouse(MBT_MIDDLE, position);
}

void BrowserPrototype::rightClick(const QPoint &position)
{
	THIS_BROWSER_P()->pressMouse(MBT_RIGHT, position);
	THIS_BROWSER_P()->releaseMouse(MBT_RIGHT, position);
}

void BrowserPrototype::pressLeft(const QPoint &position)
{
	THIS_BROWSER_P()->pressMouse(MBT_LEFT, position);
}

void BrowserPrototype::pressMiddle(const QPoint &position)
{
	THIS_BROWSER_P()->pressMouse(MBT_MIDDLE, position);
}

void BrowserPrototype::pressRight(const QPoint &position)
{
	THIS_BROWSER_P()->pressMouse(MBT_RIGHT, position);
}

void BrowserPrototype::releaseLeft(const QPoint &position)
{
	THIS_BROWSER_P()->releaseMouse(MBT_LEFT, position);
}

void BrowserPrototype::releaseMiddle(const QPoint &position)
{
	THIS_BROWSER_P()->releaseMouse(MBT_MIDDLE, position);
}

void BrowserPrototype::releaseRight(const QPoint &position)
{
	THIS_BROWSER_P()->releaseMouse(MBT_RIGHT, position);
}

void BrowserPrototype::moveMouse(const QPoint &position)
{
	THIS_BROWSER_P()->moveMouse(position);
}

void BrowserPrototype::scrollWheel(const QPoint &position, int delta_x, int delta_y)
{
	THIS_BROWSER_P()->scrollWheel(position, delta_x, delta_y);
}

void BrowserPrototype::executeJavascript(const QString &javascript_code)
{
	THIS_BROWSER_P()->executeJavascript(javascript_code);
}

QString BrowserPrototype::toString() const
{
	if (THIS_BROWSER_P()->name().isEmpty())
	{
		return QString("Browser(size: %0x%1)").arg(THIS_BROWSER_P()->size().width(), THIS_BROWSER_P()->size().height());
	}
	return QString("Browser(size: %0x%1, name: %2)").arg(THIS_BROWSER_P()->size().width(), THIS_BROWSER_P()->size().height()).arg(THIS_BROWSER_P()->name());
}
