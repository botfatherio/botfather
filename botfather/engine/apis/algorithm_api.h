#ifndef BFP_ENGINE_APIS_ALGORITHM_API_H
#define BFP_ENGINE_APIS_ALGORITHM_API_H

#include "abstract_api.h"

class AlgorithmApi : public AbstractAPI
{
	Q_OBJECT

public:
	using AbstractAPI::AbstractAPI;

	Q_INVOKABLE QScriptValue dijkstra(const QScriptValue &graph, const QString &start, const QString &dest);
};

#endif // BFP_ENGINE_APIS_ALGORITHM_API_H
