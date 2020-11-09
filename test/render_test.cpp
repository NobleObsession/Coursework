#include <gtest/gtest.h>
#include <fstream>

#include "../src/include/json.h"
#include "../src/include/Database.h"
#include "../src/include/render.h"

using namespace std;
namespace RenderTest{

TEST(Render, FoundMinAndMaxCoordinatesOfStops){
    ifstream fin("queries_for_render_test.json");
    Json::Document doc = Json::Load(fin);
    fin.close();

    auto main_map = doc.GetRoot().AsMap();

    Database database;

    CreateDatabase(main_map, database);

    const StopMap& stops = database.stops_;
    MinMaxCoordinates coord = FindMinMaxCoordinatesOfStops(stops);

    EXPECT_EQ(stops.at("Hotel").GetCoordinates().latitude,coord.min_lat);
    EXPECT_EQ(stops.at("Homeland").GetCoordinates().latitude,coord.max_lat);

    EXPECT_EQ(stops.at("Homeland").GetCoordinates().longitude,coord.min_long);
    EXPECT_EQ(stops.at("Seagull").GetCoordinates().longitude,coord.max_long);
}

TEST(Render, ZoomCoordinatesToMapRepresentation){
    ifstream fin("queries_for_render_test.json");
    Json::Document doc = Json::Load(fin);
    fin.close();

    auto main_map = doc.GetRoot().AsMap();

    Database database;

    CreateDatabase(main_map, database);

    const StopMap& stops = database.stops_;
    MinMaxCoordinates coordinates = FindMinMaxCoordinatesOfStops(stops);
    RenderSettings settings = GetRenderSettings(main_map);
    Render render(settings.width, settings.height, settings.padding, coordinates);
    double found_x = render.ConvertLongitudeToX(stops.at("Maritime").GetCoordinates().longitude);
    double found_y = render.ConvertLatitudeToY(stops.at("Maritime").GetCoordinates().latitude);

    const double expected_x = 202.7046433700865;
    const double expected_y = 725.1651507897237;
    EXPECT_FLOAT_EQ(expected_x, found_x);
    EXPECT_FLOAT_EQ(expected_y, found_y);
}

TEST(Render, ZoomCoordinates_AllStopsAtTheSamePlace_ResultEqToPadding){
    const double shared_latitude = 40;
    const double shared_longitude = 50;
    Stop first_stop("first_stop", shared_latitude, shared_longitude);
    Stop second_stop("second_stop", shared_latitude, shared_longitude);
    Database database;

    AddStop(first_stop, database);
    AddStop(second_stop, database);
    const StopMap& stops = database.stops_;

    MinMaxCoordinates coordinates = FindMinMaxCoordinatesOfStops(stops);
    const double padding = 70;
    RenderSettings settings = {1200, 1200, padding};
    Render render(settings.width, settings.height, settings.padding, coordinates);
    double found_x = render.ConvertLongitudeToX(stops.at("first_stop").GetCoordinates().longitude);
    double found_y = render.ConvertLatitudeToY(stops.at("first_stop").GetCoordinates().latitude);

    EXPECT_FLOAT_EQ(padding, found_x);
    EXPECT_FLOAT_EQ(padding, found_y);
}

TEST(Render, ZoomCoordinates_AllStopsHaveSameLatitude_LatitudeEqToPadding){
    const double shared_latitude = 40;
    Stop first_stop("first_stop", shared_latitude, 50);
    Stop second_stop("second_stop", shared_latitude, 60);
    Database database;

    AddStop(first_stop, database);
    AddStop(second_stop, database);
    const StopMap& stops = database.stops_;

    MinMaxCoordinates coordinates = FindMinMaxCoordinatesOfStops(stops);
    const double padding = 50;
    RenderSettings settings = {1200, 1200, padding};
    Render render(settings.width, settings.height, settings.padding, coordinates);

    double found_x = render.ConvertLongitudeToX(stops.at("second_stop").GetCoordinates().longitude);
    double found_y = render.ConvertLatitudeToY(stops.at("second_stop").GetCoordinates().latitude);

    const double expected_x = 1150;
    EXPECT_FLOAT_EQ(expected_x, found_x);
    EXPECT_FLOAT_EQ(padding, found_y);
}

TEST(Render, ZoomCoordinates_AllStopsHaveSameLongitude_LongitudeEqToPadding){
    const double shared_longitude = 40;
    Stop first_stop("first_stop", 50, shared_longitude);
    Stop second_stop("second_stop", 60, shared_longitude);
    Database database;

    AddStop(first_stop, database);
    AddStop(second_stop, database);
    const StopMap& stops = database.stops_;

    MinMaxCoordinates coordinates = FindMinMaxCoordinatesOfStops(stops);
    const double padding = 50;
    RenderSettings settings = {1200, 1200, padding};
    Render render(settings.width, settings.height, settings.padding, coordinates);
    double found_x = render.ConvertLongitudeToX(stops.at("first_stop").GetCoordinates().longitude);
    double found_y = render.ConvertLatitudeToY(stops.at("first_stop").GetCoordinates().latitude);

    const double expected_y = 1150;
    EXPECT_FLOAT_EQ(expected_y, found_y);
    EXPECT_FLOAT_EQ(padding, found_x);
}


}
