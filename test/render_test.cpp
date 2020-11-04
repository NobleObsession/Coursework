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
/*
    map<string, Json::Node> main_map = doc.GetRoot().AsMap();
    Database database;

    CreateDatabase(main_map, database);

    const StopMap& stops = database.stops_;
    MinMaxCorrdinates coord = FindMinMaxCoordinatesOfStops(stops);

    EXPECT_EQ(stops.at("Гостиница Сочи").GetCoordinates().latitude,coord.min_lat);
    EXPECT_EQ(stops.at("Санаторий Родина").GetCoordinates().latitude,coord.max_lat);

    EXPECT_EQ(stops.at("Санаторий Родина").GetCoordinates().longitude,coord.min_long);
    EXPECT_EQ(stops.at("Улица Лизы Чайкиной").GetCoordinates().longitude,coord.max_long);
*/
}
}
