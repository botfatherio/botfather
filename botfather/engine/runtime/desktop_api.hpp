#ifndef BFP_ENGINE_APIS_DESKTOP_API_H
#define BFP_ENGINE_APIS_DESKTOP_API_H

#include <QImage>
#include <QObject>
#include <QScriptEngine>
#include <exception>

#include "../desktop/desktop.hpp"
#include "abstract_api.hpp"

class UnknownMouseButtonError : public std::exception {};

class DesktopAPI : public AbstractAPI {
    Q_OBJECT
   public:
    DesktopAPI(Engine *bot, QObject *parent = nullptr);

    // Returns a Image showing a screenshot of the desktop.
    Q_INVOKABLE QImage takeScreenshot();

    // Returns the size of the users desktop (including multiple screens).
    Q_INVOKABLE QSize getSize();
    Q_INVOKABLE QRect getRect();

    // legacy
    Q_INVOKABLE void leftClick(const QPoint &position);
    Q_INVOKABLE void middleClick(const QPoint &position);
    Q_INVOKABLE void rightClick(const QPoint &position);

    Q_INVOKABLE void pressMouse(const QPoint &position,
                                const QString &button = "left");
    Q_INVOKABLE void holdMouse(const QPoint &position,
                               const QString &button = "left");
    Q_INVOKABLE void releaseMouse(const QPoint &position,
                                  const QString &button = "left");

    Q_INVOKABLE void pressKey(const QString &key);
    Q_INVOKABLE void holdKey(const QString &key);
    Q_INVOKABLE void releaseKey(const QString &key);

    // Q_INVOKABLE void dragAndDrop(const QPoint &from, const QPoint &to);
    Q_INVOKABLE void warpCursor(const QPoint &position);
    Q_INVOKABLE QPoint getCursorPosition();

    Q_INVOKABLE void scroll(int units, bool horizontal = false);

   protected:
    Desktop::MouseButtons mouseButtonFromString(const QString &button) const;

   private:
    Desktop *desktop;
};

#endif  // BFP_ENGINE_APIS_DESKTOP_API_H
