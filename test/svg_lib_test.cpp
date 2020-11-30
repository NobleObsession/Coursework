#include <gtest/gtest.h>

#include <sstream>
#include "../src/include/svg_lib.h"

using namespace Svg;
using namespace std;

namespace SvgTest{

namespace{
const string kFill = "fill=\"none\" ";
const string kStroke = "stroke=\"none\" ";
const string kStrokeWidth = "stroke-width=\"1.000000\" ";
const string kDefaultCircleParams = "cx=\"0.000000\" cy=\"0.000000\" r=\"1.000000\" ";
const string kDefaultTextParams = "x=\"0.000000\" y=\"0.000000\" dx=\"0.000000\" "
                           "dy=\"0.000000\" font-size=\"1\" ";
}

TEST(Svg, Render_WithoutObject){
    Document svg;
    stringstream sstream;
    svg.Render(sstream);

    string actual_output = sstream.str();
    string expected_output(kDefaultStartTag + kDefaultEndTag);
    EXPECT_EQ(expected_output, actual_output);
}

TEST(Svg, Render_PolylineWithDefaultParams){
    Document svg;
    svg.Add(Polyline{});
    stringstream sstream;
    svg.Render(sstream);

    string actual_output = sstream.str();
    string expected_output(kDefaultStartTag +
                           "<polyline " + kFill + kStroke + kStrokeWidth
                           + "/>" + kDefaultEndTag);
    EXPECT_EQ(expected_output, actual_output);
}

TEST(Svg, Render_CircleWithDefaultParams){
    Document svg;
    svg.Add(Circle{});
    stringstream sstream;
    svg.Render(sstream);

    string actual_output = sstream.str();
    string expected_output(kDefaultStartTag +
                           "<circle " +
                           kDefaultCircleParams +
                           kFill + kStroke + kStrokeWidth +
                           "/>" + kDefaultEndTag);
    EXPECT_EQ(expected_output, actual_output);
}

TEST(Svg, Render_PolylineWithFillColorParam){
    Document svg;
    svg.Add(Polyline{}.SetFillColor(Rgb{140, 198, 63}));
    stringstream sstream;
    svg.Render(sstream);

    string actual_output = sstream.str();
    string expected_output(kDefaultStartTag +
                           "<polyline fill=\"rgb(140,198,63)\" " + kStroke + kStrokeWidth
                           + "/>" + kDefaultEndTag);
    EXPECT_EQ(expected_output, actual_output);
}

TEST(Svg, Render_PolylineWithStrokeColorParam){
    Document svg;
    svg.Add(Polyline{}.SetStrokeColor(Rgb{140, 198, 63}));
    stringstream sstream;
    svg.Render(sstream);

    string actual_output = sstream.str();
    string expected_output(kDefaultStartTag +
                           "<polyline " + kFill + "stroke=\"rgb(140,198,63)\" "
                           + kStrokeWidth + "/>" + kDefaultEndTag);
    EXPECT_EQ(expected_output, actual_output);
}

TEST(Svg, Render_PolylineWithStrokeWidthParam){
    Document svg;
    svg.Add(Polyline{}.SetStrokeWidth(16));
    stringstream sstream;
    svg.Render(sstream);

    string actual_output = sstream.str();
    string expected_output(kDefaultStartTag +
                           "<polyline " + kFill + kStroke +
                           "stroke-width=\"16.000000\" />" + kDefaultEndTag);
    EXPECT_EQ(expected_output, actual_output);
}

TEST(Svg, Render_PolylineWithLineCapParam){
    Document svg;
    svg.Add(Polyline{}.SetStrokeLineCap("round"));
    stringstream sstream;
    svg.Render(sstream);

    string actual_output = sstream.str();
    string expected_output(kDefaultStartTag +
                           "<polyline " + kFill + kStroke + kStrokeWidth +
                           "stroke-linecap=\"round\" />" + kDefaultEndTag);
    EXPECT_EQ(expected_output, actual_output);
}

TEST(Svg, Render_PolylineWithLineJoinParam){
    Document svg;
    svg.Add(Polyline{}.SetStrokeLineJoin("bevel"));
    stringstream sstream;
    svg.Render(sstream);

    string actual_output = sstream.str();

    string expected_output(kDefaultStartTag +
                           "<polyline " + kFill + kStroke + kStrokeWidth +
                           "stroke-linejoin=\"bevel\" />" + kDefaultEndTag);
    EXPECT_EQ(expected_output, actual_output);
}

TEST(Svg, Render_AddTwoDifferentObjectsWithDefaultParams){
    Document svg;
    svg.Add(Polyline{});
    svg.Add(Circle{});
    stringstream sstream;
    svg.Render(sstream);

    const std::string default_polyline = "<polyline " + kFill + kStroke +
            kStrokeWidth + "/>";
    const std::string default_circle = "<circle " + kDefaultCircleParams + kFill + kStroke +
            kStrokeWidth + "/>";
    string actual_output = sstream.str();
    string expected_output(kDefaultStartTag +
                           default_polyline + default_circle + kDefaultEndTag);
    EXPECT_EQ(expected_output, actual_output);
}

TEST(Svg, Render_Polyline_AddPoints){
    Document svg;
    svg.Add(Polyline{}.AddPoint({50, 50}).AddPoint({250, 250}));
    stringstream sstream;
    svg.Render(sstream);

    string actual_output = sstream.str();
    string expected_output(kDefaultStartTag +
                           "<polyline points=\"50.000000,50.000000 250.000000,250.000000 \" " + kFill + kStroke
                           + kStrokeWidth + "/>" + kDefaultEndTag);
    EXPECT_EQ(expected_output, actual_output);
}

TEST(Svg, Render_PolylineWithFill_AddOnePoint){
    Document svg;
    svg.Add(Polyline{}.SetFillColor("red").AddPoint({50, 50}));
    stringstream sstream;
    svg.Render(sstream);

    string actual_output = sstream.str();
    string expected_output(kDefaultStartTag +
                           "<polyline points=\"50.000000,50.000000 \" fill=\"red\" " + kStroke
                           + kStrokeWidth + "/>" + kDefaultEndTag);
    EXPECT_EQ(expected_output, actual_output);
}

TEST(Svg, Render_CircleWithFill_SetRadius){
    Document svg;
    svg.Add(Circle{}.SetFillColor("red").SetRadius(6));
    stringstream sstream;
    svg.Render(sstream);

    string actual_output = sstream.str();
    string expected_output(kDefaultStartTag +
                           "<circle cx=\"0.000000\" cy=\"0.000000\" r=\"6.000000\""
                           " fill=\"red\" " + kStroke
                           + kStrokeWidth + "/>" + kDefaultEndTag);
    EXPECT_EQ(expected_output, actual_output);
}

TEST(Svg, Render_CircleWithFill_SetCenter){
    Document svg;
    svg.Add(Circle{}.SetFillColor("red").SetCenter({50, 50}));
    stringstream sstream;
    svg.Render(sstream);

    string actual_output = sstream.str();
    string expected_output(kDefaultStartTag +
                           "<circle cx=\"50.000000\" cy=\"50.000000\" r=\"1.000000\""
                           " fill=\"red\" " + kStroke
                           + kStrokeWidth + "/>" + kDefaultEndTag);
    EXPECT_EQ(expected_output, actual_output);
}

TEST(Svg, Render_TextWithDefaultParams){
    Document svg;
    svg.Add(Text{});
    stringstream sstream;
    svg.Render(sstream);

    string actual_output = sstream.str();
    string expected_output(kDefaultStartTag +
                           "<text " + kDefaultTextParams + kFill + kStroke
                           + kStrokeWidth + "></text>" + kDefaultEndTag);
    EXPECT_EQ(expected_output, actual_output);
}

TEST(Svg, Render_Text_SetPoint_SetOffset_SetFontSize){
    Document svg;
    svg.Add(Text{}.SetPoint({250, 250})
            .SetOffset({10, -10})
            .SetFontSize(20));
    stringstream sstream;
    svg.Render(sstream);

    string actual_output = sstream.str();
    string expected_output(kDefaultStartTag +
                           "<text " + "x=\"250.000000\" y=\"250.000000\" dx=\"10.000000\" "
                                      "dy=\"-10.000000\" font-size=\"20\" " + kFill
                           + kStroke + kStrokeWidth + "></text>" + kDefaultEndTag);
    EXPECT_EQ(expected_output, actual_output);
}

TEST(Svg, Render_Text_SetFontFamily){
    Document svg;
    svg.Add(Text{}.SetFontFamily("Verdana"));
    stringstream sstream;
    svg.Render(sstream);

    string actual_output = sstream.str();
    string expected_output(kDefaultStartTag +
                           "<text " + kDefaultTextParams + "font-family=\"Verdana\" " + kFill
                           + kStroke + kStrokeWidth + "></text>" + kDefaultEndTag);
    EXPECT_EQ(expected_output, actual_output);
}

TEST(Svg, Render_Text_SetData){
    Document svg;
    svg.Add(Text{}.SetData("C++"));
    stringstream sstream;
    svg.Render(sstream);

    string actual_output = sstream.str();
    string expected_output(kDefaultStartTag +
                           "<text " + kDefaultTextParams + kFill
                           + kStroke + kStrokeWidth + ">C++</text>" + kDefaultEndTag);
    EXPECT_EQ(expected_output, actual_output);
}

TEST(Svg, Render_PolylineWithRgbAFill){
    Document svg;
    svg.Add(Polyline{}.SetFillColor(RgbA({255, 255, 255, 0.85})));
    stringstream sstream;
    svg.Render(sstream);
    string actual_output = sstream.str();
    string expected_output(kDefaultStartTag +
                           "<polyline fill=\"rgba(255,255,255,0.850000)\" " + kStroke
                           + kStrokeWidth + "/>" + kDefaultEndTag);
    EXPECT_EQ(expected_output, actual_output);
}
}
