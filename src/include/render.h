#pragma once

#include "../include/Database.h"

struct MinMaxCoordinates{
    double min_lat = 0;
    double max_lat = 0;
    double min_long = 0;
    double max_long = 0;
};

MinMaxCoordinates FindMinMaxCoordinatesOfStops(const StopMap& map);

struct RenderSettings{
    double width;
    double height;
    double padding;
};

RenderSettings GetRenderSettings(const std::map<std::string, Json::Node>& main_map);

class Render{
public:
    Render(double width, double height, double padding, MinMaxCoordinates coord);
    double ConvertLongitudeToX(double stop_longitude);
    double ConvertLatitudeToY(double stop_latitude);
private:
    double GetWidthZoomCoef();
    double GetHeightZoomCoef();

    double width_;
    double heigth_;
    double padding_;
    double zoom_coef_;
    MinMaxCoordinates min_max_coord_;
};
