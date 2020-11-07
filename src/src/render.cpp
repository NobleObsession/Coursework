#include "../include/render.h"
#include "../include/Database.h"
#include "../include/Stop_Bus.h"

MinMaxCorrdinates FindMinMaxCoordinatesOfStops(const StopMap& map){
    MinMaxCorrdinates result;
    for(auto stop = map.begin(); stop!= map.end(); ++stop){
        auto stop_coord = stop->second.GetCoordinates();
        if(stop == map.begin()){
           result.min_lat = stop_coord.latitude;
           result.max_lat = stop_coord.latitude;
           result.min_long = stop_coord.longitude;
           result.max_long = stop_coord.longitude;
           continue;
        }
        if(result.min_lat > stop_coord.latitude){
            result.min_lat = stop_coord.latitude;
        }
        if(result.max_lat < stop_coord.latitude){
            result.max_lat = stop_coord.latitude;
        }
        if(result.min_long > stop_coord.longitude){
            result.min_long = stop_coord.longitude;
        }
        if(result.max_long < stop_coord.longitude){
            result.max_long = stop_coord.longitude;
        }
    }
    return result;
}
