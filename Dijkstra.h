#pragma once

#include <set>
#include <vector>

#include "DataStorage_with_utilities.h"
#include "Edge.h"


class CachedGraph{
public:
	CachedGraph():edges(nullptr){}
	CachedGraph(std::vector<Edge>* processed_edges, std::vector<double> computed_path, std::vector<Edge*> prev_edge):
		edges(processed_edges), shortest_path(computed_path), previous_edge(prev_edge){}
	std::pair<double, std::vector<Edge>> GetOutputOfGraph(size_t from, size_t to);

private:
	std::vector<Edge>* edges;
	std::vector<double> shortest_path;
	std::vector<Edge*> previous_edge;
};

UnordMapNode CreateGraphOutput(const std::pair<double, std::vector<Edge>>& cached_graph_output,
		double wait_time, const NumericNamesManager& num_names);


class Graph{
public:
	Graph(const std::vector<std::vector<size_t>>& vertexes_with_incident_edges, std::vector<Edge>* edges_ptr);
	CachedGraph BuildRoute(size_t from);

private:
	std::vector<std::vector<size_t>>vertexes_with_incident_edges;
	std::vector<Edge>* edges;
	std::vector<double> shortest_path;
	std::vector<bool> visited_vertexes;
	std::vector<Edge*> previous_edge;
	std::set<std::pair<double, size_t>> not_visited_vertexes;
};

std::pair<std::vector<std::vector<size_t>>, std::vector<Edge>> BuildGraph(std::vector<std::vector<Edge>>& routes_with_edges, size_t num_vertexes, double wait_time, double velocity);

