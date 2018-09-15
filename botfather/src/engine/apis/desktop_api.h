#ifndef BFP_ENGINE_APIS_DESKTOP_API_H
#define BFP_ENGINE_APIS_DESKTOP_API_H

#include <QObject>
#include <QScriptEngine>
#include <QImage>
#include "abstract_api.h"

class Desktop;

class DesktopAPI : public AbstractAPI
{
	Q_OBJECT
public:
	DesktopAPI(Bot *bot, QObject *parent = nullptr);
	
	// Returns a Image showing a screenshot of the desktop.
	Q_INVOKABLE QImage takeScreenshot();
	
	// Returns the size of the users desktop (including multiple screens).
	Q_INVOKABLE QSize getSize();
	
	Q_INVOKABLE void leftClick(const QPoint &position);
	Q_INVOKABLE void middleClick(const QPoint &position);
	Q_INVOKABLE void rightClick(const QPoint &position);
	
	Q_INVOKABLE void pressKey(const QString &key);
	Q_INVOKABLE void holdKey(const QString &key);
	Q_INVOKABLE void releaseKey(const QString &key);
	
	Q_INVOKABLE void warpCursor(const QPoint &position);
	Q_INVOKABLE QPoint getCursorPosition();
	
	Q_INVOKABLE bool findAndClick(const QImage &tpl, double threshold = 0.8, int button = 1);
	Q_INVOKABLE QScriptValue findMatches(const QImage &tpl, double threshold = 0.8, int max_matches = -1);
	Q_INVOKABLE QScriptValue findMatch(const QImage &tpl, double threshold = 0.8);
	
private:
	Desktop *desktop;
};

#endif // BFP_ENGINE_APIS_DESKTOP_API_H
