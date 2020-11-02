#include <unordered_map>
#include <cmath>
#include <memory>

#include "../include/DataStorage_with_utilities.h"
#include "../include/Stop_Bus_Parsers.h"

using namespace std;

void AddStop(const Stop& stop, DataStorage& ds){
	auto found_stop = ds.stops_.find(stop.GetStopName());
	if(found_stop != ds.stops_.end()){
		found_stop->second.SetCoordinates(stop.GetCoordinates());
		found_stop->second.SetDistances(stop.GetDistances());
	}else{
		ds.stops_[stop.GetStopName()] = stop;
	}
}

void AddBus(const Bus& bus, DataStorage& ds){
	ds.buses_[bus.GetBusName()] = bus;
	vector<string>stops = bus.GetStops();
	for(auto& s:stops){
		ds.stops_[s].AddBus(bus.GetBusName());
	}
}

void AddNotExistedStops(const vector<string>& stops, const string& route, DataStorage& ds){
	for(auto stop:stops){
		if(ds.stops_.count(stop) == 0){
			Stop empty_stop({stop, 0, 0});
			ds.stops_[stop] = empty_stop;
			ds.stops_[stop].AddBus(route);
		}
	}
}

void CreateDatabase(const map<string, Json::Node>& main_map, DataStorage& storage){

    for(auto& req: main_map.at("base_requests").AsArray()){
        map<string, Json::Node> map_with_requests = req.AsMap();
        if(map_with_requests.at("type").AsString() == "Stop"){

            Stop s = ParseStop(map_with_requests);
            AddStop(s, storage);
        }else if(map_with_requests.at("type").AsString() == "Bus"){

            Bus b = ParseBus(map_with_requests);
            AddBus(b, storage);
            AddNotExistedStops(b.GetStops(), b.GetBusName(), storage);
        }
    }
}

void FindBus(const string& name, UnordMapNode& output_node, const DataStorage& ds, ComputeLengthNode& len_node){

	if(ds.buses_.count(name) != 0){
		output_node.AddPair("stop_count", make_shared<IntNode>(ds.buses_.at(name).NumAllStops()));
		output_node.AddPair("unique_stop_count", make_shared<IntNode>(ds.buses_.at(name).NumUniqueStops()));

		ComputedRouteParams params = len_node.ComputeRouteLen(name, ds);
		output_node.AddPair("route_length", make_shared<DoubleNode>(params.length));
		output_node.AddPair("curvature", make_shared<DoubleNode>(params.curvature));
	}else{
		output_node.AddPair("error_message", make_shared<StringNode>("not found"));
	}
}

void FindStop(const string& stop, UnordMapNode& output_node, const DataStorage& ds){

	if(ds.stops_.count(stop) != 0){
		set<string>set_of_buses = ds.stops_.at(stop).GetBuses();
		VectorNode buses;
		for(auto& bus:set_of_buses){
			buses.AddNode(make_shared<StringNode>(bus));
		}
		output_node.AddPair("buses", make_shared<VectorNode>(buses));
	}else{
		output_node.AddPair("error_message", make_shared<StringNode>("not found"));
	}
}


ComputedRouteParams ComputeLengthNode::ComputeRouteLen(const string& bus_name, const DataStorage& ds){
	auto found_params = route_cache.find(bus_name);
	if(found_params != route_cache.end()){
		return (*found_params).second;
	}else{
		vector<string>stops_of_route = ds.buses_.at(bus_name).GetStops();

		double len_act = 0;
		double len_teor = 0;

		for(auto it = stops_of_route.begin(); it < prev(stops_of_route.end()); ++it){
			auto current_stop_iter = it;
			auto next_stop_iter = next(it);
			if(ds.stops_.at(*current_stop_iter).GetDistances().count(*next_stop_iter) == 0){
				swap(current_stop_iter, next_stop_iter);
			}
			len_act += ds.stops_.at(*current_stop_iter).GetDistances()[*next_stop_iter];
			len_teor += ComputeLen(ds.stops_.at(*current_stop_iter).GetRadCoordinates(),
					ds.stops_.at(*next_stop_iter).GetRadCoordinates());
		}
		ComputedRouteParams params;
		params.curvature = len_act/len_teor;
		params.length = len_act;
		route_cache[bus_name] = params;
		return params;
	}
}

double ComputeLengthNode::ComputeLen(const Coord& lhs, const Coord& rhs){
	return acos((sin(lhs.lat_rad) * sin(rhs.lat_rad)) +
				(cos(lhs.lat_rad) * cos(rhs.lat_rad) *
				cos(abs(lhs.long_rad - rhs.long_rad)))) * earth_rad;
}

void NumericNamesManager::SetNumericNamesOfStops(const DataStorage& ds){
	size_t counter = 0;
	for(auto& stop:ds.stops_){
		stop_name_to_size_t[stop.first] = counter;
		size_t_to_stop_name[counter] = stop.first;
		counter += 1;
	}
}

size_t NumericNamesManager::GetNumericName (const std::string& string_name) const{
	return stop_name_to_size_t.at(string_name);
}

string NumericNamesManager::GetStringName (const size_t numeric_name) const{
	return size_t_to_stop_name.at(numeric_name);
}

size_t NumericNamesManager::GetNumNumericNames() const{
	return size_t_to_stop_name.size();
}

void NumericNamesManager::SetAdditionalVertex(size_t num_vertex, const std::string& name_of_stop){
	size_t_to_stop_name[num_vertex] = name_of_stop;
}

pair<vector<vector<Edge>>, size_t> GetAllEdgesWithWeights (double velocity,
		const DataStorage& ds, NumericNamesManager& num_names){
	/*This function is intended to return all original edges (without repetitions) for each bus route
	 * with number of original stops (without repetitions too). To track this, there is class NumericNamesManager,
	 * where size of stop_name_to_size_t corresponds to number of original stops, whereas
	 * size of size_t_to_stop_name != stop_name_to_size_t because of
	 * addition of utility stops (i.e. vertexes), which are needed for computation purposes, not for output of results.*/

	vector<vector<Edge>>routes_with_edges;
	for(auto& bus:ds.buses_){
		vector<Edge> edges;
		int distance;

		vector<string>stops_of_route = ds.buses_.at(bus.first).GetStops();

		for(auto name_of_stop = stops_of_route.begin(); name_of_stop < prev(stops_of_route.end()); ++name_of_stop){
			if(ds.stops_.at(*name_of_stop).GetDistances().count(*next(name_of_stop)) == 0){
				distance = ds.stops_.at(*next(name_of_stop)).GetDistances()[*name_of_stop];
			}else{
				distance = ds.stops_.at(*name_of_stop).GetDistances()[*next(name_of_stop)];
			}

			if(name_of_stop == prev(prev(stops_of_route.end()))){
				size_t new_number_of_last_vertex = num_names.GetNumNumericNames();
				num_names.SetAdditionalVertex(new_number_of_last_vertex, *next(name_of_stop));
				edges.push_back({num_names.GetNumericName(*name_of_stop), new_number_of_last_vertex, (distance/velocity), bus.first});
			}else{
				edges.push_back({num_names.GetNumericName(*name_of_stop), num_names.GetNumericName(*next(name_of_stop)), (distance/velocity), bus.first});
			}
		}
		routes_with_edges.push_back(edges);
	}

	return make_pair(routes_with_edges, num_names.GetNumNumericNames());
}

