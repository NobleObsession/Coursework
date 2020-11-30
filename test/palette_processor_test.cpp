#include <gtest/gtest.h>
#include <fstream>

#include "../src/include/json.h"
#include "../src/include/Database.h"
#include "../src/src/palette_processor.cpp"

using namespace std;
namespace PaletteProcessorTest{

TEST(PaletteProcessor, LoadPalette){
    ifstream fin("queries_for_render_test.json");
    Json::Document doc = Json::Load(fin);
    fin.close();

    auto main_map = doc.GetRoot().AsMap();
    std::vector<Svg::Color> palette = GetPalette(main_map);

    EXPECT_EQ("green", palette[0].color_);
    EXPECT_EQ("rgb(255,160,0)", palette[1].color_);
    EXPECT_EQ("red", palette[2].color_);
}

}
