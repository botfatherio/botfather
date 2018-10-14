#ifndef BFP_ENGINE_APIS_PATH_FINDER_API_H
#define BFP_ENGINE_APIS_PATH_FINDER_API_H

#include "abstract_api.h"
#include "../modules/algorithms/dijkstra.h"

class PathFinderApi : public AbstractAPI
{
	Q_OBJECT

public:
	using AbstractAPI::AbstractAPI;

	Q_INVOKABLE QScriptValue findShortestPath(const QScriptValue &graph, const QString &start, const QString &dest);
};

#endif // BFP_ENGINE_APIS_PATH_FINDER_API_H
