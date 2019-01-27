#ifndef CANVAS_GFX_COLOR_H
#define CANVAS_GFX_COLOR_H
#include <cstdint>

#include "glm/glm.hpp"

namespace GFX {

class Color : public glm::vec4 {
  public:
    explicit Color(uint32_t argb)
      : glm::vec4(float((argb >> 16) & 0xFF) / 255.0f,
                  float((argb >>  8) & 0xFF) / 255.0f,
                  float((argb >>  0) & 0xFF) / 255.0f,
                  float((argb >> 24) & 0xFF) / 255.0f) {}

    explicit Color(uint8_t r, uint8_t g, uint8_t b)
      : glm::vec4(float(r) / 255.0f, float(g) / 255.0f,
                  float(b) / 255.0f, 1.0f) {}

    static const Color INDIANRED;
    static const Color LIGHTCORAL;
    static const Color SALMON;
    static const Color DARKSALMON;
    static const Color LIGHTSALMON;
    static const Color CRIMSON;
    static const Color RED;
    static const Color FIREBRICK;
    static const Color DARKRED;
    static const Color PINK;
    static const Color LIGHTPINK;
    static const Color HOTPINK;
    static const Color DEEPPINK;
    static const Color MEDIUMVIOLETRED;
    static const Color PALEVIOLETRED;
    static const Color CORAL;
    static const Color TOMATO;
    static const Color ORANGERED;
    static const Color DARKORANGE;
    static const Color ORANGE;
    static const Color GOLD;
    static const Color YELLOW;
    static const Color LIGHTYELLOW;
    static const Color LEMONCHIFFON;
    static const Color LIGHTGOLDENRODYELLOW;
    static const Color PAPAYAWHIP;
    static const Color MOCCASIN;
    static const Color PEACHPUFF;
    static const Color PALEGOLDENROD;
    static const Color KHAKI;
    static const Color DARKKHAKI;
    static const Color LAVENDER;
    static const Color THISTLE;
    static const Color PLUM;
    static const Color VIOLET;
    static const Color ORCHID;
    static const Color FUCHSIA;
    static const Color MAGENTA;
    static const Color MEDIUMORCHID;
    static const Color MEDIUMPURPLE;
    static const Color REBECCAPURPLE;
    static const Color BLUEVIOLET;
    static const Color DARKVIOLET;
    static const Color DARKORCHID;
    static const Color DARKMAGENTA;
    static const Color PURPLE;
    static const Color INDIGO;
    static const Color SLATEBLUE;
    static const Color DARKSLATEBLUE;
    static const Color MEDIUMSLATEBLUE;
    static const Color GREENYELLOW;
    static const Color CHARTREUSE;
    static const Color LAWNGREEN;
    static const Color LIME;
    static const Color LIMEGREEN;
    static const Color PALEGREEN;
    static const Color LIGHTGREEN;
    static const Color MEDIUMSPRINGGREEN;
    static const Color SPRINGGREEN;
    static const Color MEDIUMSEAGREEN;
    static const Color SEAGREEN;
    static const Color FORESTGREEN;
    static const Color GREEN;
    static const Color DARKGREEN;
    static const Color YELLOWGREEN;
    static const Color OLIVEDRAB;
    static const Color OLIVE;
    static const Color DARKOLIVEGREEN;
    static const Color MEDIUMAQUAMARINE;
    static const Color DARKSEAGREEN;
    static const Color LIGHTSEAGREEN;
    static const Color DARKCYAN;
    static const Color TEAL;
    static const Color AQUA;
    static const Color CYAN;
    static const Color LIGHTCYAN;
    static const Color PALETURQUOISE;
    static const Color AQUAMARINE;
    static const Color TURQUOISE;
    static const Color MEDIUMTURQUOISE;
    static const Color DARKTURQUOISE;
    static const Color CADETBLUE;
    static const Color STEELBLUE;
    static const Color LIGHTSTEELBLUE;
    static const Color POWDERBLUE;
    static const Color LIGHTBLUE;
    static const Color SKYBLUE;
    static const Color LIGHTSKYBLUE;
    static const Color DEEPSKYBLUE;
    static const Color DODGERBLUE;
    static const Color CORNFLOWERBLUE;
    static const Color ROYALBLUE;
    static const Color BLUE;
    static const Color MEDIUMBLUE;
    static const Color DARKBLUE;
    static const Color NAVY;
    static const Color MIDNIGHTBLUE;
    static const Color CORNSILK;
    static const Color BLANCHEDALMOND;
    static const Color BISQUE;
    static const Color NAVAJOWHITE;
    static const Color WHEAT;
    static const Color BURLYWOOD;
    static const Color TAN;
    static const Color ROSYBROWN;
    static const Color SANDYBROWN;
    static const Color GOLDENROD;
    static const Color DARKGOLDENROD;
    static const Color PERU;
    static const Color CHOCOLATE;
    static const Color SADDLEBROWN;
    static const Color SIENNA;
    static const Color BROWN;
    static const Color MAROON;
    static const Color WHITE;
    static const Color SNOW;
    static const Color HONEYDEW;
    static const Color MINTCREAM;
    static const Color AZURE;
    static const Color ALICEBLUE;
    static const Color GHOSTWHITE;
    static const Color WHITESMOKE;
    static const Color SEASHELL;
    static const Color BEIGE;
    static const Color OLDLACE;
    static const Color FLORALWHITE;
    static const Color IVORY;
    static const Color ANTIQUEWHITE;
    static const Color LINEN;
    static const Color LAVENDERBLUSH;
    static const Color MISTYROSE;
    static const Color GAINSBORO;
    static const Color LIGHTGRAY;
    static const Color SILVER;
    static const Color DARKGRAY;
    static const Color GRAY;
    static const Color DIMGRAY;
    static const Color LIGHTSLATEGRAY;
    static const Color SLATEGRAY;
    static const Color DARKSLATEGRAY;
    static const Color BLACK;
};

}  // namespace GFX

#endif // CANVAS_GFX_COLOR_H
