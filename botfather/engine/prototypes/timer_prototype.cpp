#include "timer_prototype.h"

#define THIS_TIMER() qscriptvalue_cast<QElapsedTimer>(thisObject())
#define THIS_TIMER_P() qscriptvalue_cast<QElapsedTimer*>(thisObject())

QScriptValue TimerPrototype::constructor(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() == 0) {
		return engine->toScriptValue(QElapsedTimer());
	}

	NO_MATCHING_CTOR("Timer", TIMER_PROTOTYPE_DOCS)
}

bool TimerPrototype::isRunning() const
{
	return THIS_TIMER().isValid();
}

void TimerPrototype::start()
{
	THIS_TIMER_P()->start();
}

qint64 TimerPrototype::restart()
{
	QElapsedTimer* timer(THIS_TIMER_P());
	if (timer->isValid())
	{
		return timer->restart();
	}
	timer->start();
	return 0;
}

void TimerPrototype::stop()
{
	THIS_TIMER_P()->invalidate();
}

qint64 TimerPrototype::elapsed() const
{
	QElapsedTimer timer(THIS_TIMER());
	if (timer.isValid())
	{
		return timer.elapsed();
	}
	return 0;
}

bool TimerPrototype::hasExpired(qint64 timeout_in_ms) const
{
	return THIS_TIMER().hasExpired(timeout_in_ms);
}

qint64 TimerPrototype::msecsTo(const QElapsedTimer &other) const
{
	// impl => The return value is undefined if this object or other were invalidated.
	QElapsedTimer timer(THIS_TIMER());
	if (timer.isValid() && other.isValid())
	{
		return timer.msecsTo(other);
	}
	return -1;
}

QString TimerPrototype::toString() const
{
	return QString("Timer(running: %1, elapsed: %2)").arg(isRunning() ? "true" : "false").arg(elapsed());
}
