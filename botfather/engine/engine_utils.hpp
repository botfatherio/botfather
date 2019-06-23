#ifndef ENGINE_UTILS_HPP
#define ENGINE_UTILS_HPP

#include <QCborValue>
#include <QScriptValue>

namespace EngineUtils
{
	QScriptValue convertToQScriptValue(QScriptEngine *engine, const QCborValue &cbor_value);
}

#endif // ENGINE_UTILS_HPP
