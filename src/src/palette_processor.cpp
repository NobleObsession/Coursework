#include "../include/json.h"
#include "../include/Database.h"
#include "../include/svg_lib.h"

std::vector<Svg::Color> GetPalette(const std::map<std::string, Json::Node>& main_map){
    auto json_settings = main_map.at("render_settings").AsMap();
    std::vector<Svg::Color> palette;

    auto json_colors = json_settings["color_palette"].AsArray();
    for(auto color:json_colors){
        if (std::holds_alternative<std::string>(color.GetBase())){
            palette.push_back(color.AsString());
        }else{
            auto rgb_values = color.AsArray();
            if(3 == rgb_values.size()){
                palette.push_back(Svg::Rgb{rgb_values[0].AsInt(),
                       rgb_values[1].AsInt(), rgb_values[2].AsInt()});
            }else{
                palette.push_back(Svg::RgbA{rgb_values[0].AsInt(),
                       rgb_values[1].AsInt(), rgb_values[2].AsInt(), rgb_values[3].AsDouble()});
            }
        }
    }
    return palette;
}
