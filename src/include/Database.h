#pragma once

#include <vector>

#include "../include/Stop_Bus.h"
#include "../include/json.h"

using StopMap = std::unordered_map<std::string, Stop>;

struct Database{
    std::unordered_map<std::string, Bus> buses_;
    StopMap stops_;
};

void AddBus(const Bus& route, Database& ds);

void AddNotExistedStops(const std::vector<std::string>& stops, const std::string& bus_name, Database& ds);

void AddStop(const Stop& stop, Database& ds);

void CreateDatabase(const std::map<std::string, Json::Node>& main_map,
                    Database& storage);
