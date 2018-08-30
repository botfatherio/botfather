#include "path_finder_api.h"
#include <QScriptValueIterator>
#include <QDebug>

QScriptValue PathFinderApi::findShortestPath(const QScriptValue &graph_object, const QString &start, const QString &dest)
{
	// The script value must be an object. The objects properties are the names of the graphs nodes.
	// The properties/nodes values are the nodes edges. Those must be objects aswell.
	// The edges objects propertie names are the edges destinations node name and the
	// according values are the edges weight or distance.

	if (!graph_object.isObject())
	{
		return context()->throwError(QScriptContext::TypeError, "graph must be an object");
	}

	QMap<QString, QMap<QString, int>> graph;

	QScriptValueIterator graph_nodes_it(graph_object);
	while (graph_nodes_it.hasNext())
	{
		graph_nodes_it.next();
		QString node_name = graph_nodes_it.name();

		if (!graph_nodes_it.value().isObject())
		{
			return context()->throwError(QScriptContext::TypeError, "graph must contain objects");
		}

		QMap<QString, int> edges;

		QScriptValueIterator node_edges_it(graph_nodes_it.value());
		while (node_edges_it.hasNext())
		{
			node_edges_it.next();
			QString edge_destination_node_name = node_edges_it.name();
			int edge_weight = static_cast<int>(node_edges_it.value().toNumber());
			edges[edge_destination_node_name] = edge_weight;
		}

		graph[node_name] = edges;
	}

	Dijkstra dijkstra(graph);
	return engine()->toScriptValue(dijkstra.findShortestPath(start, dest));
}
