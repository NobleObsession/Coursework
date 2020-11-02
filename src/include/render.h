#pragma once

#include <map>
#include <unordered_map>


#include "../include/json.h"

class Render{
public:
    Render(const std::map<std::string, Json::Node>& render_settings):
        settings_(render_settings)  {}
    void ConvertCoordinates();

private:
    std::map<string, Json::Node> settings_;
};

