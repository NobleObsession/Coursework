#include "../include/render.h"
#include "../include/Database.h"
#include "../include/Stop_Bus.h"

MinMaxCorrdinates FindMinMaxCoordinatesOfStops(const StopMap& map){
    MinMaxCorrdinates coord;
    for(const auto& pair:map){
        auto coordinates = pair.second.GetCoordinates();
        coord.min_lat < coordinates.latitude ? coord.min_lat : coord.min_lat = coordinates.latitude;
        coord.max_lat > coordinates.latitude ? coord.max_lat : coord.max_lat = coordinates.latitude;
        coord.min_long < coordinates.longitude ? coord.min_long : coord.min_long = coordinates.longitude;
        coord.max_long > coordinates.longitude ? coord.max_long : coord.max_long = coordinates.longitude;
    }
    return coord;
}
