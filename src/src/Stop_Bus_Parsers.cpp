#include <iostream>
#include <sstream>
#include <math.h>
#include <variant>
#include <iomanip>

#include "../include/Stop_Bus_Parsers.h"

using namespace std;

Stop ParseStop(map<std::string, Json::Node>& my_map){

	string name = my_map["name"].AsString();
	double latitude = my_map["latitude"].AsDouble();
	double longitude = my_map["longitude"].AsDouble();

	unordered_map<string, int>distance_to_stops;
	for(auto& elem:my_map["road_distances"].AsMap()){
		distance_to_stops[elem.first] = static_cast<int>(elem.second.AsDouble());
	}
	return {name, latitude, longitude, distance_to_stops};
}

Bus ParseBus(map<string, Json::Node>& my_map){
	bool is_linear;
	my_map["is_roundtrip"].AsBool() ? is_linear = false : is_linear = true;

	string name = my_map["name"].AsString();

	vector<string>stops;
	unordered_set<string>unique_stops;
	for(auto& elem:my_map["stops"].AsArray()){
		stops.push_back(elem.AsString());
		unique_stops.insert(elem.AsString());
	}

	vector<string>stops_copy = stops;
	if(is_linear){
		for(auto it = next(stops_copy.rbegin()); it != stops_copy.rend(); ++it){
			stops.push_back(*it);
		}
	}

	Bus r(name, stops, unique_stops);
	return r;
}
