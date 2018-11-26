#ifndef BFP_ENGINE_PROTOTYPES_TIMER_PROTOTYPE_H
#define BFP_ENGINE_PROTOTYPES_TIMER_PROTOTYPE_H
#define TIMER_PROTOTYPE_DOCS "https://botfather.io/docs/apiref/timer/"

#include "abstract_prototype.h"
#include <QElapsedTimer>

Q_DECLARE_METATYPE(QElapsedTimer)
Q_DECLARE_METATYPE(QElapsedTimer*)

class TimerPrototype : public AbstractPrototype
{
	Q_OBJECT

public:
	static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
	using AbstractPrototype::AbstractPrototype;

	// The methods of http://doc.qt.io/qt-5/qelapsedtimer.html have been modified.
	// Methods which require the QElapsedTimer to be valid return a default value
	// if the timer isn't valid.
	// And both invalidate() and isValid() have been renamed, as their names might
	// confuse newcomers.

	// Returns false if the timer has never been started or stopped by a call to stop().
	// http://doc.qt.io/qt-5/qelapsedtimer.html#isValid
	Q_INVOKABLE bool isRunning() const;

	// Starts this timer. Once started, a timer value can be checked with elapsed() or msecsSinceReference().
	// http://doc.qt.io/qt-5/qelapsedtimer.html#start
	Q_INVOKABLE void start();

	// Restarts the timer and returns the time elapsed since the previous start.
	// The return value is 0 if this object is invalidated, the timer though will be started anyway.
	// http://doc.qt.io/qt-5/qelapsedtimer.html#restart
	Q_INVOKABLE qint64 restart();

	// Stops the timer and thus makes running() return false.
	// http://doc.qt.io/qt-5/qelapsedtimer.html#invalidate
	Q_INVOKABLE void stop();

	// Returns the number of milliseconds since this QElapsedTimer was last started.
	// The return value is 0 if this object is invalidated.
	// http://doc.qt.io/qt-5/qelapsedtimer.html#elapsed
	Q_INVOKABLE qint64 elapsed() const;

	// Returns true if this QElapsedTimer has already expired by timeout milliseconds (that is, more than timeout milliseconds have elapsed).
	// The value of timeout can be -1 to indicate that this timer does not expire, in which case this function will always return false.
	// http://doc.qt.io/qt-5/qelapsedtimer.html#hasExpired
	Q_INVOKABLE bool hasExpired(qint64 timeout_in_ms) const;

	// Returns the number of milliseconds between this QElapsedTimer and other.
	// The return value is -1 if this object or other were invalidated.
	// http://doc.qt.io/qt-5/qelapsedtimer.html#msecsTo
	Q_INVOKABLE qint64 msecsTo(const QElapsedTimer &other) const;

	Q_INVOKABLE QString toString() const;
};

#endif // BFP_ENGINE_PROTOTYPES_TIMER_PROTOTYPE_H
