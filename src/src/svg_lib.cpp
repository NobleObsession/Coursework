#include <memory>
#include <vector>
#include <string>

namespace Svg{

const std::string kDefaultStartTag =
"<?xml version=\"1.0\" encoding=\"UTF-8\" ?><svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">";
const std::string kDefaultEndTag = "</svg>";
const double kDefaultStrokeWidth = 1.0;

struct Point{
    double x;
    double y;
};

struct Rgb{
    int red;
    int green;
    int blue;
};

struct Color{
    Color():color_("none"){}
    Color(const std::string& color):color_(color) {}
    Color(const Rgb& color):
        color_("rgb(" +
              std::to_string(color.red) + ',' +
              std::to_string(color.green) + ',' +
              std::to_string(color.blue) + ")") {}

    std::string color_;
};

const static Color NoneColor = Color();

struct GeneralProperties {
    GeneralProperties():fill_(NoneColor), stroke_(NoneColor),
    stroke_width_(kDefaultStrokeWidth) {}

    Color fill_;
    Color stroke_;
    double stroke_width_;
    std::string stroke_linecap_;
    std::string stroke_linejoin_;

    std::string ToString() const{
        std::string result;
        result += "fill=\"" + fill_.color_ + "\" ";
        result += "stroke=\"" + stroke_.color_ + "\" ";
        result += "stroke-width=\"" + std::to_string(stroke_width_) + "\" ";
        if(!stroke_linecap_.empty()){
          result += "stroke-linecap=\"" + stroke_linecap_ + "\" ";
        }
        if(!stroke_linejoin_.empty()){
          result += "stroke-linejoin=\"" + stroke_linejoin_ + "\" ";
        }
        return result;
    }
};

/*abstract builder class*/
template<typename T> class Object{
public:
    //shared setters

    T& SetFillColor(const Color& color){
        general_properties_.fill_ = color;
        return *(static_cast<T*>(this));
    }

    T& SetFillColor(const std::string& color){
        general_properties_.fill_ = Color(color);
        return *(static_cast<T*>(this));
    }

    T& SetStrokeColor(const Color& color){
        general_properties_.stroke_ = color;
        return *(static_cast<T*>(this));
    }

    T& SetStrokeColor(const std::string& color){
        general_properties_.stroke_ = Color(color);
        return *(static_cast<T*>(this));
    }

    T& SetStrokeWidth(double width){
        general_properties_.stroke_width_ = width;
        return *(static_cast<T*>(this));
    }
    T& SetStrokeLineCap(const std::string& linecap){
        general_properties_.stroke_linecap_ = linecap;
        return *(static_cast<T*>(this));
    }
    T& SetStrokeLineJoin(const std::string& linejoin){
        general_properties_.stroke_linejoin_ = linejoin;
        return *(static_cast<T*>(this));
    }

    virtual std::string GetProperties() const {
        return general_properties_.ToString();
    }

protected:
    GeneralProperties general_properties_;
};

/*concrete builder*/
class Polyline: public Object<Polyline>{
public:
    std::string GetProperties() const override{
        std::string stringified_properties("<polyline ");

        if(!points_.empty()){
            stringified_properties += "points=\"";
            for(const auto point:points_){
                stringified_properties += std::to_string(point.x) + ','
                         + std::to_string(point.y) + ' ';
             }
             stringified_properties += "\" ";
         }
         stringified_properties += general_properties_.ToString();
         stringified_properties += "/>";
         return stringified_properties;
    }
    Polyline& AddPoint(Point point){
      points_.push_back(point);
      return *this;
    }
private:
    std::vector<Point> points_;
};

struct CircleProperties{
    CircleProperties():center_x(0), center_y(0), radius(1.0) {}

    double center_x;
    double center_y;
    double radius;

    std::string ToString() const{
        std::string result;
        result += "cx=\"" + std::to_string(center_x) + "\" ";
        result += "cy=\"" + std::to_string(center_y) + "\" ";
        result += "r=\"" + std::to_string(radius) + "\" ";
        return result;
    }

};

/*concrete builder*/
class Circle: public Object<Circle>{
public:
    std::string GetProperties() const override{
            std::string stringified_properties("<circle ");
            stringified_properties += circle_properties_.ToString();
            stringified_properties += general_properties_.ToString();
            stringified_properties += "/>";
            return stringified_properties;
        }
    Circle& SetCenter(Point point){
      circle_properties_.center_x = point.x;
      circle_properties_.center_y = point.y;
      return *this;
    }
    Circle& SetRadius(double radius){
      circle_properties_.radius = radius;
      return *this;
    }
private:
    CircleProperties circle_properties_;
};

struct TextProperties{
    TextProperties():coord_x(0), coord_y(0), offset_x(0),
               offset_y(0), font_size(1) {}

    double coord_x;
    double coord_y;
    double offset_x;
    double offset_y;
    uint32_t font_size;
    std::string font_family;

    std::string ToString() const{
        std::string result;
        result += "x=\"" + std::to_string(coord_x) + "\" ";
        result += "y=\"" + std::to_string(coord_y) + "\" ";
        result += "dx=\"" + std::to_string(offset_x) + "\" ";
        result += "dy=\"" + std::to_string(offset_y) + "\" ";
        result += "font-size=\"" + std::to_string(font_size) + "\" ";
        if(!font_family.empty()){
            result += "font-family=\"" + font_family + "\" ";
        }
        return result;
    }
};

/*concrete builder*/
class Text: public Object<Text>{
public:
    std::string GetProperties() const override{
            std::string stringified_properties("<text ");
            stringified_properties += text_properties_.ToString();
            stringified_properties += general_properties_.ToString();
            stringified_properties += ">";
            if(!text_data_.empty()){
                stringified_properties +=  text_data_;
            }
            stringified_properties += "</text>";
            return stringified_properties;
        }
    Text& SetPoint(Point point){
        text_properties_.coord_x = point.x;
        text_properties_.coord_y = point.y;
        return *this;
      }
    Text& SetOffset(Point point){
        text_properties_.offset_x = point.x;
        text_properties_.offset_y = point.y;
        return *this;
      }
    Text& SetFontSize(uint32_t font_size){
        text_properties_.font_size = font_size;
        return *this;
      }
    Text& SetFontFamily(const std::string& font_family){
        text_properties_.font_family = font_family;
        return *this;
      }
    Text& SetData(const std::string& data){
        text_data_ = data;
        return *this;
      }
private:
    TextProperties text_properties_;
    std::string text_data_;
};

/*Director class, intended to use instances of concrete builders
 * classes*/

class Document{
public:
    void Add(const Polyline& object){
        properties_.push_back(object.GetProperties());
    }
    void Add(const Circle& object){
        properties_.push_back(object.GetProperties());
    }
    void Add(const Text& object){
        properties_.push_back(object.GetProperties());
    }
    void Render(std::ostream& out){
        out << kDefaultStartTag;

        for(auto property:properties_){
            out << property;
        }
        out << kDefaultEndTag;
    }
   private:
    std::vector<std::string> properties_;
};

}
