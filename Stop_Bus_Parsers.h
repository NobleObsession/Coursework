#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <set>
#include <utility>

#include "json.h"

struct Coord{
	Coord(double latitude, double longitude):lat_rad(latitude), long_rad(longitude){
		lat_rad = (lat_rad * pi) / 180;
		long_rad = (long_rad * pi) / 180;
		}
	double lat_rad;
	double long_rad;
	const double pi = 3.1415926535;
};


class Stop{
public:
	Stop(std::string& name, double latitude, double longitude, std::unordered_map<std::string, int> dist_map = {}):
		name_(name), latitude_(latitude), longitude_(longitude), dist_to_other(dist_map) {}

	Stop():latitude_(0), longitude_(0){}

	void AddBus(const std::string& bus_name){
		buses_.insert(bus_name);
	}
	std::set<std::string> GetBuses() const {return buses_;}
	std::string GetStopName() const {return name_;}
	Coord GetRadCoordinates() const {return {latitude_, longitude_};}
	std::pair<double, double> GetCoordinates() const {return std::make_pair(latitude_, longitude_);}
	std::unordered_map<std::string, int> GetDistances() const{return dist_to_other;}

	void SetCoordinates(std::pair<double, double> coord){
		latitude_ = coord.first;
		longitude_ = coord.second;
	}
	void SetDistances(const std::unordered_map<std::string, int>& dist){dist_to_other = dist;}


private:
	std::string name_;
	double latitude_;
	double longitude_;
	std::set<std::string>buses_;
	std::unordered_map<std::string, int>dist_to_other;
};

class Bus{
public:

	Bus() = default;

	explicit Bus(std::string number, std::vector<std::string>& stops,
			std::unordered_set<std::string>& unique_stops):
		name_(number), stops_(stops), unique_stops_(unique_stops) {
	}

	size_t NumAllStops() const{return stops_.size();}
	size_t NumUniqueStops() const{return unique_stops_.size();}
	std::string GetBusName() const{return name_;}
	std::vector<std::string>GetStops() const {return stops_;}


private:
	std::string name_;
	std::vector<std::string>stops_;
	std::unordered_set<std::string>unique_stops_;
};


Stop ParseStop(std::map<std::string, Json::Node>& my_map);
Bus ParseBus(std::map<std::string, Json::Node>& my_map);

