#include "../../Public/Geometry/Vec4_t.h"
#include "../../Public/Geometry/Vec3_t.h"

Vec4_t::Vec4_t(double _x, double _y, double _z, double _w){
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

Vec4_t::Vec4_t(const std::vector<double> &v){
    x = v[0];
    y = v[1];
    z = v[2];
    w = v[3];
}

Vec4_t::~Vec4_t()= default;

Vec4_t Vec4_t::operator +(const Vec4_t &other) const{
    return {x + other.x, y + other.y, z+other.z, w+other.w};
}

Vec4_t Vec4_t::operator -(const Vec4_t &other) const{
    return {x - other.x, y - other.y, z - other.z, w - other.w};
}

Vec4_t Vec4_t::operator *(const double factor) const{
    return {x * factor, y * factor, z * factor, w * factor};
}

Vec4_t Vec4_t::operator /(const double factor) const{
    return {x / factor, y / factor, z / factor, w / factor};
}

double Vec4_t::length() const {
    return sqrt( x * x + y * y + z * z + w * w );
}

double Vec4_t::getX() const{
    return x;
}

double Vec4_t::getY() const{
    return y;
}
double Vec4_t::getZ() const{
    return z;
}
double Vec4_t::getW() const{
    return w;
}

Vec3_t Vec4_t::toVec3() const{
    return {x, y, z};
}