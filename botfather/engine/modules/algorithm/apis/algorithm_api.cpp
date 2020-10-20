#include "algorithm_api.hpp"

#include <QDebug>
#include <QMapIterator>
#include <QScriptValueIterator>

#include "../dijkstra.hpp"

QScriptValue AlgorithmAPI::dijkstra(const QScriptValue &graph_object,
                                    const QString &start, const QString &dest) {
    // The script value must be an object. The objects properties are the names
    // of the graphs nodes. The properties/nodes values are the nodes edges.
    // Those must be objects aswell. The edges objects propertie names are the
    // edges destinations node name and the according values are the edges
    // weight or distance.

    if (!graph_object.isObject()) {
        return context()->throwError(QScriptContext::TypeError,
                                     "graph must be an object");
    }

    QMap<QString, QMap<QString, int>> graph;

    QScriptValueIterator graph_nodes_it(graph_object);
    while (graph_nodes_it.hasNext()) {
        graph_nodes_it.next();
        QString node_name = graph_nodes_it.name();

        // NOTE: Turns out arrays are also objects. So check that they are not.
        if (!graph_nodes_it.value().isObject() ||
            graph_nodes_it.value().isArray()) {
            return context()->throwError(QScriptContext::TypeError,
                                         "graph must contain node objects");
        }

        QMap<QString, int> edges;

        QScriptValueIterator node_edges_it(graph_nodes_it.value());
        while (node_edges_it.hasNext()) {
            node_edges_it.next();
            QString edge_destination_node_name = node_edges_it.name();

            if (!node_edges_it.value().isNumber()) {
                QString message = QString("edge weight of %1 must be a number")
                                      .arg(edge_destination_node_name);
                return context()->throwError(QScriptContext::TypeError,
                                             message);
            }

            int edge_weight =
                static_cast<int>(node_edges_it.value().toNumber());
            edges[edge_destination_node_name] = edge_weight;
        }

        graph[node_name] = edges;
    }

    // Destination node existance check
    QMapIterator<QString, QMap<QString, int>> graph_it(graph);
    QList<QString> node_names = graph.keys();

    while (graph_it.hasNext()) {
        graph_it.next();
        QString start_node_name = graph_it.key();
        QMap<QString, int> edges = graph_it.value();

        for (const QString &dest_node_name : edges.keys()) {
            if (!node_names.contains(dest_node_name)) {
                QString message =
                    QString(
                        "Node %1 leads to node %2, but node %2 does not exist")
                        .arg(start_node_name, dest_node_name);
                return context()->throwError(QScriptContext::ReferenceError,
                                             message);
            }
        }
    }

    Dijkstra dijkstra(graph);
    return engine()->toScriptValue(dijkstra.findShortestPath(start, dest));
}
