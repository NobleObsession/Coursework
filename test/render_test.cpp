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


}
