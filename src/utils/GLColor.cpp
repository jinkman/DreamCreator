#include "Types.h"

namespace DM {

GLColor GLColor::deserialization(const nlohmann::json &obj) {
    return GLColor(obj[0].get<float>(), obj[1].get<float>(), obj[2].get<float>(), obj[3].get<float>());
}

nlohmann::json GLColor::serialization() {
    return nlohmann::json::array({r, g, b, a});
}

GLColor::GLColor(float red, float green, float blue, float alpha) :
    r(red), g(green), b(blue), a(alpha) {
}

GLColor::GLColor(float red, float green, float blue) :
    r(red), g(green), b(blue) {
}

GLColor::GLColor(float v) :
    r(v), g(v), b(v), a(1.0f) {
}

GLColor::GLColor() {
}

GLColor::~GLColor() {
}

float GLColor::red() const {
    return r;
}

float GLColor::green() const {
    return g;
}

float GLColor::blue() const {
    return b;
}

float GLColor::alpha() const {
    return a;
}

} // namespace DM