#include <unordered_map>
#include <cmath>
#include <memory>

#include "../include/Database.h"

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

void AddStop(const Stop& stop, Database& ds){
        auto found_stop = ds.stops_.find(stop.GetStopName());
        if(found_stop != ds.stops_.end()){
                found_stop->second.SetCoordinates(stop.GetCoordinates());
                found_stop->second.SetDistances(stop.GetDistances());
        }else{
                ds.stops_[stop.GetStopName()] = stop;
        }
}

void AddBus(const Bus& bus, Database& ds){
        ds.buses_[bus.GetBusName()] = bus;
        vector<string>stops = bus.GetStops();
        for(auto& s:stops){
                ds.stops_[s].AddBus(bus.GetBusName());
        }
}

void AddNotExistedStops(const vector<string>& stops, const string& route, Database& ds){
        for(auto stop:stops){
                if(ds.stops_.count(stop) == 0){
                        Stop empty_stop({stop, 0, 0});
                        ds.stops_[stop] = empty_stop;
                        ds.stops_[stop].AddBus(route);
                }
        }
}

void CreateDatabase(const map<string, Json::Node>& main_map, Database& storage){

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
