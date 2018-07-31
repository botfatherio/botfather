#ifndef BFP__DESKTOP__DESKTOP_API_H
#define BFP__DESKTOP__DESKTOP_API_H

#include <QObject>
#include <QScriptEngine>

class Bot;
class Desktop;
class Image;

class DesktopAPI : public QObject
{
	Q_OBJECT
public:
	explicit DesktopAPI(Bot* bot_p, QScriptEngine* engine_p);
	static void enable(Bot* bot_p, QScriptEngine* engine_p);
	
	// Returns a Image showing a screenshot of the desktop.
	Q_INVOKABLE QScriptValue takeScreenshot();
	
	// Returns the width of the users screen.
	Q_INVOKABLE int getWidth();
	
	// Returns the height of the users screen.
	Q_INVOKABLE int getHeight();
	
	Q_INVOKABLE void leftClick(int x, int y);
	Q_INVOKABLE void middleClick(int x, int y);
	Q_INVOKABLE void rightClick(int x, int y);
	
	Q_INVOKABLE void pressKey(QString key);
	Q_INVOKABLE void holdKey(QString key);
	Q_INVOKABLE void releaseKey(QString key);
	
	Q_INVOKABLE void warpCursor(int x, int y);
	Q_INVOKABLE QScriptValue getCursorPosition();
	
	Q_INVOKABLE bool findAndClick(Image* tpl, double threshold = 0.8, int button = 1);
	
	Q_INVOKABLE QScriptValue findMatches(Image* tpl, double threshold = 0.8, int max_matches = 7);
	Q_INVOKABLE QScriptValue findMatch(Image* tpl, double threshold = 0.8);
	
private:
	Bot *m_bot_p;
	QScriptEngine *m_engine_p;
	Desktop *desktop;
};

#endif // BFP__DESKTOP__DESKTOP_API_H