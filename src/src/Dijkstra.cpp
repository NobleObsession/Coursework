#include <algorithm>

#include "../include/Dijkstra.h"
#include "../include/DataStorage_with_utilities.h"

using namespace std;

const static int MAX_VALUE = 1000000;

Graph::Graph(const vector<vector<size_t>>& vertexes, vector<Edge>* edges):
	vertexes_with_incident_edges(vertexes), edges(edges) {
	shortest_path.assign(vertexes_with_incident_edges.size(), MAX_VALUE);
	visited_vertexes.assign(vertexes.size(), false);
	previous_edge.assign(vertexes.size(), nullptr);
}

CachedGraph Graph::BuildRoute(size_t from){
	for(size_t i = 0; i < vertexes_with_incident_edges.size(); ++i){
		if(i != from){
			not_visited_vertexes.emplace(make_pair(MAX_VALUE, i));
		}else{
			not_visited_vertexes.emplace(make_pair(0, i));
		}
	}

	pair<double, size_t> current_vertex;

	while(!not_visited_vertexes.empty()){
		current_vertex = *(not_visited_vertexes.begin());
		not_visited_vertexes.erase(not_visited_vertexes.begin());
		visited_vertexes[current_vertex.second] = true;
		shortest_path[current_vertex.second] = current_vertex.first;
		for(size_t incident_edge: vertexes_with_incident_edges[current_vertex.second]){
			if(visited_vertexes[(*edges)[incident_edge].to] == false){
				if((shortest_path[(*edges)[incident_edge].from] + (*edges)[incident_edge].weight) < shortest_path[(*edges)[incident_edge].to]){
					not_visited_vertexes.erase(make_pair(shortest_path[(*edges)[incident_edge].to], (*edges)[incident_edge].to));
					shortest_path[(*edges)[incident_edge].to] = shortest_path[(*edges)[incident_edge].from] + (*edges)[incident_edge].weight;
					previous_edge[(*edges)[incident_edge].to] = &(*edges)[incident_edge];
					not_visited_vertexes.emplace(make_pair(shortest_path[(*edges)[incident_edge].to], (*edges)[incident_edge].to));
				}
			}
		}
	}
	return {edges, move(shortest_path), move(previous_edge)};
}


pair<double, vector<Edge>>  CachedGraph::GetOutputOfGraph(size_t from, size_t to){
	vector<Edge>route_edges;

	if(shortest_path[to] != MAX_VALUE){
		size_t current_vertex = to;
		while(current_vertex != from){
			route_edges.push_back(*previous_edge[current_vertex]);
			current_vertex = (*previous_edge[current_vertex]).from;
		}
		reverse(route_edges.begin(), route_edges.end());
	}
	return make_pair(shortest_path[to], route_edges);
}

UnordMapNode CreateGraphOutput(const pair<double, vector<Edge>>& cached_graph_output, double wait_time,
		const NumericNamesManager& num_names){
	UnordMapNode output_node;
	if(cached_graph_output.first == MAX_VALUE){
		output_node.AddPair("error_message", make_shared<StringNode>("not found"));
	}else{
		output_node.AddPair("total_time", make_shared<DoubleNode>(cached_graph_output.first));
		VectorNode items;
		for(auto& edge:cached_graph_output.second){
			if(edge.num_stops == 0){
				continue;
			}else{
				UnordMapNode wait_map, stop_map;
				wait_map.AddPair("time", make_shared<DoubleNode>(wait_time));
				wait_map.AddPair("type", make_shared<StringNode>("Wait"));
				wait_map.AddPair("stop_name", make_shared<StringNode>(num_names.GetStringName(edge.from)));
				items.AddNode(make_shared<UnordMapNode>(wait_map));

				stop_map.AddPair("span_count", make_shared<IntNode>(edge.num_stops));
				stop_map.AddPair("bus", make_shared<StringNode>(edge.route));
				stop_map.AddPair("type", make_shared<StringNode>("Bus"));
				stop_map.AddPair("time", make_shared<DoubleNode>(edge.weight - wait_time));
				items.AddNode(make_shared<UnordMapNode>(stop_map));
			}
		}
		output_node.AddPair("items", make_shared<VectorNode>(items));
	}
	return output_node;
}

pair<vector<vector<size_t>>, vector<Edge>> BuildGraph(vector<vector<Edge>>& routes_with_edges, size_t num_vertexes, double wait_time, double velocity){

	vector<vector<size_t>> vertexes(num_vertexes);
	vector<Edge> edges;
	int edge_counter = 0;

	for(auto route:routes_with_edges){
		vector<Edge> all_possible_edges_per_route;
		for(auto it = route.begin(); it < route.end(); ++it){
			int stop_counter = 0;
			(*it).weight += wait_time;
			(*it).num_stops = ++stop_counter;
			all_possible_edges_per_route.push_back(*it);
			vertexes[(*it).from].push_back(edge_counter++);

			auto next_it = next(it);
			while(next_it != route.end()){
				all_possible_edges_per_route.push_back({(*it).from, (*next_it).to,
					all_possible_edges_per_route.back().weight + (*next_it).weight, (*it).route,
					++stop_counter});
				vertexes[(*it).from].push_back(edge_counter++);
				next_it = next(next_it);
			}
			all_possible_edges_per_route.push_back({(*prev(route.end())).to, (*route.begin()).from,
				0, (*it).route, 0});

			vertexes[(*prev(route.end())).to].push_back(edge_counter++);
		}
		edges.insert(edges.end(), all_possible_edges_per_route.begin(), all_possible_edges_per_route.end());
	}
	return {vertexes, edges};
}
