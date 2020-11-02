#pragma once

#include <variant>
#include <unordered_map>

#include "Edge.h"
#include "Stop_Bus_Parsers.h"
#include "output_node.h"
#include "../include/json.h"

struct DataStorage;

void AddBus(const Bus& route, DataStorage& ds);

void AddNotExistedStops(const std::vector<std::string>& stops, const std::string& bus_name, DataStorage& ds);

void AddStop(const Stop& stop, DataStorage& ds);

void CreateDatabase(const std::map<std::string, Json::Node>& main_map,
                    DataStorage& storage);

struct ComputedRouteParams{
	double curvature;
	double length;
};

class ComputeLengthNode{
public:
	ComputedRouteParams ComputeRouteLen(const std::string& bus_name, const DataStorage& ds);
private:
	double ComputeLen(const Coord& lhs, const Coord& rhs);
	const int earth_rad = 6371000;
	std::unordered_map<std::string, ComputedRouteParams>route_cache;
};

void FindBus(const std::string& bus_name, UnordMapNode& map_node, const DataStorage& ds, ComputeLengthNode& len_node);
void FindStop(const std::string& stop_name, UnordMapNode& map_node, const DataStorage& ds);

class NumericNamesManager{
public:
	void SetNumericNamesOfStops(const DataStorage& ds);
	size_t GetNumericName (const std::string& string_name) const;
	std::string GetStringName (const size_t numeric_name) const;
	size_t GetNumNumericNames() const;
	void SetAdditionalVertex(size_t num_vertex, const std::string& name_of_stop);

private:
	std::unordered_map<std::string, size_t> stop_name_to_size_t;
	std::unordered_map<size_t, std::string> size_t_to_stop_name;
};

std::pair<std::vector<std::vector<Edge>>, size_t> GetAllEdgesWithWeights (double velocity,
		const DataStorage& ds, NumericNamesManager& num_names);

struct DataStorage{
	std::unordered_map<std::string, Bus> buses_;
	std::unordered_map<std::string, Stop>stops_;
};


