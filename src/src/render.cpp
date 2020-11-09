#include "../include/render.h"
#include "../include/Database.h"
#include "../include/Stop_Bus.h"
#include "../include/json.h"

MinMaxCoordinates FindMinMaxCoordinatesOfStops(const StopMap& map){
    MinMaxCoordinates result;
    for(auto stop = map.begin(); stop!= map.end(); ++stop){
        double stop_latitude = stop->second.GetCoordinates().latitude;
        double stop_longitude = stop->second.GetCoordinates().longitude;
        if(stop == map.begin()){
           result.min_lat = stop_latitude;
           result.max_lat = stop_latitude;
           result.min_long = stop_longitude;
           result.max_long = stop_longitude;
           continue;
        }
        if(result.min_lat > stop_latitude){
            result.min_lat = stop_latitude;
        }
        if(result.max_lat < stop_latitude){
            result.max_lat = stop_latitude;
        }
        if(result.min_long > stop_longitude){
            result.min_long = stop_longitude;
        }
        if(result.max_long < stop_longitude){
            result.max_long = stop_longitude;
        }
    }
    return result;
}

RenderSettings GetRenderSettings(const std::map<std::string, Json::Node>& main_map){
    auto json_settings = main_map.at("render_settings").AsMap();
    RenderSettings parsed_settings;
    parsed_settings.width = json_settings["width"].AsDouble();
    parsed_settings.height = json_settings["height"].AsDouble();
    parsed_settings.padding = json_settings["padding"].AsDouble();
    return parsed_settings;
}

double GetFinalZoom(double width_zoom, double height_zoom){
    if(width_zoom == 0 && height_zoom == 0){
        return 0;
    }
    if(width_zoom == 0){
        return height_zoom;
    }
    if(height_zoom == 0){
       return width_zoom;
    }
    if(width_zoom <= height_zoom){
       return width_zoom;
    }
    return height_zoom;
}

Render::Render(double width, double height, double padding, MinMaxCoordinates coord):
    width_(width), heigth_(height), padding_(padding), min_max_coord_(coord){

    double width_zoom = GetWidthZoomCoef();
    double height_zoom = GetHeightZoomCoef();
    zoom_coef_ = GetFinalZoom(width_zoom, height_zoom);
}

Render& Render::SetHeight(double height){
    heigth_ = height;
    return *this;
}

Render& Render::SetPadding(double padding){
    padding_ = padding;
    return *this;
}

Render& Render::SetMinMaxCoordinates(const MinMaxCoordinates& coord){
    min_max_coord_ = coord;
    return *this;
}

double Render::GetWidthZoomCoef(){
    if(min_max_coord_.max_long - min_max_coord_.min_long != 0){
        return (width_ - 2 * padding_) /
                (min_max_coord_.max_long - min_max_coord_.min_long);
    }
    return 0;
}

double Render::GetHeightZoomCoef(){
    if(min_max_coord_.max_lat - min_max_coord_.min_lat != 0){
        return (heigth_ - 2 * padding_) /
                (min_max_coord_.max_lat - min_max_coord_.min_lat);
    }
    return 0;
}

double Render::ConvertLongitudeToX(double stop_longitude){
    return (stop_longitude - min_max_coord_.min_long) * zoom_coef_ + padding_;
}

double Render::ConvertLatitudeToY(double stop_latitude){
    return (min_max_coord_.max_lat - stop_latitude) * zoom_coef_ + padding_;
}

