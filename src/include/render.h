#pragma once

#include "../include/Database.h"

struct MinMaxCorrdinates{
    double min_lat = 0;
    double max_lat = 0;
    double min_long = 0;
    double max_long = 0;
};

MinMaxCorrdinates FindMinMaxCoordinatesOfStops(const StopMap& map);
