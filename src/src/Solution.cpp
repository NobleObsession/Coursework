#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <utility>
#include <memory>
#include <fstream>

#include "../include/json.h"
#include "../include/Dijkstra.h"
#include "../include/Stop_Bus.h"
#include "../include/Cache.h"
#include "../include/DataStorage_with_utilities.h"
#include "../include/output_node.h"
#include "../include/Database.h"


using namespace std;

shared_ptr<VectorNode> ProcessStatQueries(const map<string, Json::Node>& main_map, Database& storage,
		const NumericNamesManager& names_manager, double wait_time, const Graph& initial_graph){

	vector<shared_ptr<Node>> resolved_queries;
	Cache cache;
	ComputeLengthNode len_node;

	for(auto& req: main_map.at("stat_requests").AsArray()){
		map<string, Json::Node> map_with_requests = req.AsMap();

		UnordMapNode map_node;

		if(map_with_requests.at("type").AsString() == "Stop"){
			FindStop(map_with_requests["name"].AsString(), map_node, storage);

		}else if(map_with_requests.at("type").AsString() == "Bus"){
			FindBus(map_with_requests["name"].AsString(), map_node, storage, len_node);

		}else if(map_with_requests.at("type").AsString() == "Route"){
			size_t from_size_t = names_manager.GetNumericName(map_with_requests.at("from").AsString());
			size_t to_size_t = names_manager.GetNumericName(map_with_requests.at("to").AsString());
			Route route({from_size_t, to_size_t});


			pair<bool, UnordMapNode> found_route = cache.FindRoute(route);
			if(found_route.first == true){
				map_node = move(found_route.second);
			}else{
				pair<bool, CachedGraph> found_graph = cache.FindGraph(from_size_t);
				if(found_graph.first == false){
					Graph graph = initial_graph;
					CachedGraph processed_graph = graph.BuildRoute(from_size_t);
					auto inserted_graph = cache.InsertCachedGraph(from_size_t, processed_graph);
					found_graph.second = (*inserted_graph.first).second;
				}
				UnordMapNode output_node = CreateGraphOutput(found_graph.second.GetOutputOfGraph(from_size_t, to_size_t), wait_time, names_manager);
				cache.InsertOutputNode(route, output_node);
				map_node =  move(output_node);
			}
		}
		int num_request = static_cast<int>(map_with_requests["id"].AsDouble());
		map_node.AddPair("request_id", make_shared<IntNode>(num_request));
		resolved_queries.push_back(make_shared<UnordMapNode>(map_node));
	}
	return make_shared<VectorNode>(resolved_queries);
}

void PrintQueries(const shared_ptr<VectorNode> node, ostream& out){
	node->Output(out);
}

int main(){
	Json::Document doc = Json::Load(cin);

	/*uncomment to run with input file
	ifstream fin("transport-input.json");
	Json::Document doc = Json::Load(fin);
	fin.close();*/

	map<string, Json::Node> main_map = doc.GetRoot().AsMap();

	map<string, Json::Node> settings = main_map["routing_settings"].AsMap();

	const double wait_time = settings["bus_wait_time"].AsDouble();
	const double velocity = (settings["bus_velocity"].AsDouble() * 1000)/60;

    Database storage;
	CreateDatabase(main_map, storage);

	NumericNamesManager num_names_manager;
	num_names_manager.SetNumericNamesOfStops(storage);

	pair<vector<vector<Edge>>, size_t> all_edges = GetAllEdgesWithWeights(velocity, storage, num_names_manager);

	pair<vector<vector<size_t>>, vector<Edge>> components_of_graph = BuildGraph(all_edges.first, all_edges.second, wait_time, velocity);

	Graph initial_graph(components_of_graph.first, &components_of_graph.second);


	shared_ptr<VectorNode> resolved_queries = ProcessStatQueries(main_map, storage, num_names_manager, wait_time, initial_graph);
	PrintQueries(resolved_queries, cout);

	/*uncomment to write output in file
	ofstream out_file("output.txt");
	PrintQueries(resolved_queries, out_file);
	out_file.close();*/

	return 0;
}
