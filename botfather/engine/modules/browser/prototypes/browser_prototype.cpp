#include "browser_prototype.hpp"
#include "../../../engine.hpp"
#include "../browser_util.hpp"

// We must cast to Browser* otherwise the browsers pointers won't be valid.
#define THIS_BROWSER_P() qscriptvalue_cast<Browser*>(thisObject())

QScriptValue BrowserPrototype::constructor(QScriptContext *context, QScriptEngine *engine)
{
	// Each bot/engine instance has its own browsers. When creating a browser we must
	// tell the browser host which group/bot it belongs in. The group name used here
	// is the bots ID.
	Engine *vm = qobject_cast<Engine*>(engine->parent());

	// Validate the browsers id
	QString id;
	if (context->argumentCount() >= 1 && context->argument(0).isString())
	{
		id = context->argument(0).toString();
		if (id.isEmpty())
		{
			return context->throwError(QScriptContext::Error::TypeError, "The browsers id must not be empty");
		}
	}

	// new Browser(id) with default size of 1366x768
	if (context->argumentCount() == 1 && context->argument(0).isString())
	{
		QSize default_size(1366, 768);
		Browser *browser = BrowserManager::instance()->createBrowser(vm->id(), id, default_size);
		return engine->newQObject(browser, QScriptEngine::QtOwnership);
	}

	// new Browser(id, size);
	if (context->argumentCount() == 2 && context->argument(0).isString() && isQSize(context->argument(1)))
	{
		QSize size = qscriptvalue_cast<QSize>(context->argument(1));
		if (size.width() < 400 || size.height() < 400)
		{
			return context->throwError(QScriptContext::Error::RangeError, "The browsers size must be at least 400x400");
		}
		Browser *browser = BrowserManager::instance()->createBrowser(vm->id(), id, size);
		return engine->newQObject(browser, QScriptEngine::QtOwnership);
	}

	NO_MATCHING_CTOR("Browser", BROWSER_PROTOTYPE_DOCS)
}

QImage BrowserPrototype::takeScreenshot()
{
	return THIS_BROWSER_P()->takeScreenshot();
}

QRect BrowserPrototype::getRect() const
{
	return THIS_BROWSER_P()->rect();
}

QSize BrowserPrototype::getSize() const
{
	return THIS_BROWSER_P()->size();
}

void BrowserPrototype::resize(const QSize &size)
{
	THIS_BROWSER_P()->resize(size);
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
	THIS_BROWSER_P()->pressMouse(position, Qt::LeftButton);
}

void BrowserPrototype::middleClick(const QPoint &position)
{
	THIS_BROWSER_P()->pressMouse(position, Qt::MiddleButton);
}

void BrowserPrototype::rightClick(const QPoint &position)
{
	THIS_BROWSER_P()->pressMouse(position, Qt::RightButton);
}

void BrowserPrototype::holdLeft(const QPoint &position)
{
	THIS_BROWSER_P()->holdMouse(position, Qt::LeftButton);
}

void BrowserPrototype::holdMiddle(const QPoint &position)
{
	THIS_BROWSER_P()->holdMouse(position, Qt::MiddleButton);
}

void BrowserPrototype::holdRight(const QPoint &position)
{
	THIS_BROWSER_P()->holdMouse(position, Qt::RightButton);
}

void BrowserPrototype::releaseLeft(const QPoint &position)
{
	THIS_BROWSER_P()->releaseMouse(position, Qt::LeftButton);
}

void BrowserPrototype::releaseMiddle(const QPoint &position)
{
	THIS_BROWSER_P()->releaseMouse(position, Qt::MiddleButton);
}

void BrowserPrototype::releaseRight(const QPoint &position)
{
	THIS_BROWSER_P()->releaseMouse(position, Qt::RightButton);
}

void BrowserPrototype::moveMouse(const QPoint &position)
{
	THIS_BROWSER_P()->moveMouse(position);
}

void BrowserPrototype::scrollWheel(const QPoint &position, const QPoint &delta)
{
	THIS_BROWSER_P()->scrollWheel(position, delta);
}

void BrowserPrototype::pressKey(const QString &keycode)
{
	if (!THIS_BROWSER_P()->canPressKey(keycode))
	{
		QString message = QString("Unsupported keycode. Check the keymap in the Browser documentation");
		context()->throwError(QScriptContext::TypeError, message);
		return void();
	}
	THIS_BROWSER_P()->pressKey(keycode);
}

void BrowserPrototype::holdKey(const QString &keycode)
{
	if (!THIS_BROWSER_P()->canPressKey(keycode))
	{
		QString message = QString("Unsupported keycode. Check the keymap in the Browser documentation");
		context()->throwError(QScriptContext::TypeError, message);
		return void();
	}
	THIS_BROWSER_P()->holdKey(keycode);
}

void BrowserPrototype::releaseKey(const QString &keycode)
{
	if (!THIS_BROWSER_P()->canPressKey(keycode))
	{
		QString message = QString("Unsupported keycode. Check the keymap in the Browser documentation");
		context()->throwError(QScriptContext::TypeError, message);
		return void();
	}
	THIS_BROWSER_P()->releaseKey(keycode);
}

void BrowserPrototype::enterText(const QString &text)
{
	if (!THIS_BROWSER_P()->canEnterText(text))
	{
		QString message = QString("text contains unsupported characters. Check the keymap in the Browser documentation");
		context()->throwError(QScriptContext::TypeError, message);
		return void();
	}
	THIS_BROWSER_P()->enterText(text);
}

void BrowserPrototype::executeJavascript(const QString &javascript_code)
{
	THIS_BROWSER_P()->executeJavascript(javascript_code);
}

QScriptValue BrowserPrototype::evaluateJavascript(const QString &javascript_code, int timeout_in_ms)
{
	QCborValue result;
	QVariantMap exception;
	bool timed_out;
	QString script_url("inline");

	if (THIS_BROWSER_P()->evaluateJavascript(script_url, javascript_code, timeout_in_ms, result, exception, timed_out))
	{
		return BrowserUtil::convertToQScriptValue(engine(), result);
	}
	else if (!exception.isEmpty())
	{
		int line_number = exception["line_number"].toInt();
		QString message = exception["message"].toString();
		QString msg = QString("[%1] Line %2, %3").arg(script_url).arg(line_number).arg(message);
		return context()->throwError(QScriptContext::UnknownError, msg);
	}
	else if (timed_out)
	{
		QString msg(QString("Evaluation timed out, more that %0 milliseconds passed.").arg(timeout_in_ms));
		return context()->throwError(QScriptContext::UnknownError, msg);
	}
	else
	{
		QString msg("Script execution failed.");
		return context()->throwError(QScriptContext::UnknownError, msg);
	}
}

QString BrowserPrototype::toString() const
{
	return QString("Browser(id: %0, size: %1x%2)").arg(THIS_BROWSER_P()->name()).arg(THIS_BROWSER_P()->size().width(), THIS_BROWSER_P()->size().height());
}
