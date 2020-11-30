#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <set>
#include <utility>

struct RadCoordinates{
	RadCoordinates(double latitude, double longitude):lat_rad(latitude), long_rad(longitude){
		lat_rad = (lat_rad * pi) / 180;
		long_rad = (long_rad * pi) / 180;
		}
	double lat_rad;
	double long_rad;
	const double pi = 3.1415926535;
};

struct Coordinates{
    double latitude;
    double longitude;
};

class Stop{
public:
    Stop(const std::string& name, double latitude, double longitude, std::unordered_map<std::string, int> dist_map = {}):
		name_(name), latitude_(latitude), longitude_(longitude), dist_to_other(dist_map) {}

	Stop():latitude_(0), longitude_(0){}

	void AddBus(const std::string& bus_name){
		buses_.insert(bus_name);
	}
	std::set<std::string> GetBuses() const {return buses_;}
	std::string GetStopName() const {return name_;}
	RadCoordinates GetRadCoordinates() const {return {latitude_, longitude_};}
	Coordinates GetCoordinates() const {return {latitude_, longitude_};}
	std::unordered_map<std::string, int> GetDistances() const{return dist_to_other;}

	void SetCoordinates(Coordinates coord){
		latitude_ = coord.latitude;
		longitude_ = coord.longitude;
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
