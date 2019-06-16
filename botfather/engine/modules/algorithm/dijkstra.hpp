#ifndef BFP_ENGINE_MODULES_ALGORITHMS_DIJKSTRA_H
#define BFP_ENGINE_MODULES_ALGORITHMS_DIJKSTRA_H

#include <QMap>
#include <QList>
#include <QHash>
#include <QPair>
#include <QStringList>

class Dijkstra
{
public:
        Dijkstra() {}
		explicit Dijkstra(const QMap<QString, QMap<QString, int>> &data);

        void addNode(const QString &node_name, const QMap<QString, int> &edges);
        void removeNode(const QString &node_name);
		QStringList findShortestPath(const QString &start, const QString &destination);

protected:
        // I decided to split the Dijkstra algorithm into the following methods to increase code readability.

        // Resets variables and initialises the list of unvisited nodes and their distances to the start node.
        void prepareSearchStartingFrom(const QString &start_node);

        // Sorts a list of node names by their distance to the start node stored in the
        // distances_to_startnode hashmap.
        void sortNodeListByDistance(QList<QString> &node_list);

        // Updates the distance to the start node of nodes neighboring the node currently considered closest to the start node.
        void updateNeighbourDistance(QString current_node_name, QString neighbor_node_name, int distance_to_neighbor);

        // Assembles a list of nodes leading from the start node to the destination node.
		QStringList assembleShortestPathLeadingTo(QString destination_node_name);

private:
        // The graph represented as map of nodes. Each node has a name and a map of all it's edges.
        // Each edge has a length and knows the name of it's destination node.
        // We use the QMap type for easy access to all nodes and their edges.
        QMap<QString, QMap<QString, int>> graph;

        // List of names of nodes not yet visited (rated) by the dijkstra algorithm.
        // This list will be sorted by the Dijkstra::sortNodeListByDistance method.
        // The Dijkstra algorithm allways visits the node closest to the start node.
        // Thus sorting nodes by their distance to the start node must happen somewhere.
        // Sorting maps by something other than their keys is not possible.
        // Lists don't provide an easy way to access a specific element,
        // so using lists instead of maps wasn't an option either.
        // Thats why I choosed to sort this list of unvisited nodes by the nodes
        // distance to the start node stored in the distances_to_startnode map.
        QList<QString> unvisited_nodes;

        // Hashmap storing each nodes distance to the start node. Nodes are identified by their name.
        QHash<QString, int> distances_to_startnode;

        // The Dijkstra algorithm remembers the shortest path to each visited node.
        // Thats done by remembering each nodes previous node.
        // Finally one can lookup the shortest path from the start to the destination
        // node using Dijkstra::assembleShortestPathLeadingTo().
        QHash<QString, QString> previous_nodes;
};

#endif  // BFP_ENGINE_MODULES_ALGORITHMS_DIJKSTRA_H
