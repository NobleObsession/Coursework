#include <gtest/gtest.h>
#include <fstream>
#include "../include/json.h"
#include "../include/render.h"

#include "../src/include/render.h"
#include "../src/include/DataStorage_with_utilities.h"

using namespace std;
namespace RenderTest{

TEST(Render, ConvertOnePointCoordinates){
    ifstream fin("queries_for_render_test.json");
    Json::Document doc = Json::Load(fin);
    fin.close();

    map<string, Json::Node> main_map = doc.GetRoot().AsMap();
    DataStorage storage;
    CreateDatabase(main_map, storage);

    Render render(main_map["render_settings"].AsMap());




}
}
