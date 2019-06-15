#include "dijkstra.hpp"
#include <QDebug>

Dijkstra::Dijkstra(const QMap<QString, QMap<QString, int>> &data)
{
	graph = data;
}

void Dijkstra::addNode(const QString &node_name, const QMap<QString, int> &edges)
{
	graph[node_name] = edges;
}

void Dijkstra::removeNode(const QString &node_name)
{
	graph.remove(node_name);
}

QStringList Dijkstra::findShortestPath(const QString &start, const QString &destination)
{
	prepareSearchStartingFrom(start);
	while (!unvisited_nodes.isEmpty())
	{
		// Take (visit) the unvisited node which is closest to the start node
		QString closest_node = unvisited_nodes.first();
		unvisited_nodes.removeAll(closest_node);

		if (closest_node == destination)
		{
			// The closest node is the destination node. Stop Dijkstra and return
			// the shortest path from the start node leading to the destination node.
			return assembleShortestPathLeadingTo(destination);
		}

		if (distances_to_startnode[closest_node] == std::numeric_limits<int>::max())
		{
			// There is no path from the start node to the destination node.
			// Remember: There must not only be an edge leading to the destination node,
			// but the destination node itself must be part of the graph.
			//qDebug() << "There is no path leading from" << start << "over" << closest_node << "to" << destination;
			break;
		}

		QMap<QString, int> closest_nodes_edges = graph[closest_node];
		for (const QString &neighbor_node_name : closest_nodes_edges.keys())
		{
			// Update each neighbor-nodes distance to the start node when having the
			// currently closest node to the start node in it's path.
			int distance_to_neighbor = closest_nodes_edges.value(neighbor_node_name);
			updateNeighbourDistance(closest_node, neighbor_node_name, distance_to_neighbor);
		}
	}
	return QStringList();
}

void Dijkstra::prepareSearchStartingFrom(const QString &start_node)
{
	unvisited_nodes.clear();
	distances_to_startnode.clear();
	previous_nodes.clear();

	for (const QString &key : graph.keys())
	{
		// Consider all nodes (except the start node itself) super far away from the start node.
		distances_to_startnode[key] = key == start_node ? 0 : std::numeric_limits<int>::max();

		// Put all the graphs node in the list of unvisited nodes.
		unvisited_nodes.append(key);
	}

	// Sort the list of unvisited nodes by their distance to the start node.
	// (here the start node will be the first element of the sorted list).
	sortNodeListByDistance(unvisited_nodes);
}

void Dijkstra::sortNodeListByDistance(QList<QString> &node_list)
{
	// Each nodes distance to the start node is looked up in the distances_to_startnode hasmap.
	std::sort(node_list.begin(), node_list.end(), [&](const QString &node_name_1, const QString &node_name_2){
		return distances_to_startnode[node_name_1] < distances_to_startnode[node_name_2];
	});
}

void Dijkstra::updateNeighbourDistance(QString current_node_name, QString neighbor_node_name, int distance_to_neighbor)
{
	// Possibly new shortest distance to the start node
	int alt_distance = distances_to_startnode[current_node_name] + distance_to_neighbor;

	if (alt_distance < distances_to_startnode[neighbor_node_name])
	{
		// A new shortest path back to the start node has been found!
		distances_to_startnode[neighbor_node_name] = alt_distance;
		previous_nodes[neighbor_node_name] = current_node_name;

		// Distances to the start node changed, so we have to sort the unvisited again.
		sortNodeListByDistance(unvisited_nodes);
	}
}

QStringList Dijkstra::assembleShortestPathLeadingTo(QString destination_node_name)
{
	QStringList path_from_start_to_destination;
	QString waypoint = destination_node_name;

	while (!previous_nodes[waypoint].isEmpty())
	{
		path_from_start_to_destination.push_front(waypoint);
		waypoint = previous_nodes[waypoint];
	}

	path_from_start_to_destination.push_front(waypoint);
	return path_from_start_to_destination;
}
