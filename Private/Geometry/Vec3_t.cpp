#include "../../Public/Geometry/Vec3_t.h"
#include "../../Public/Geometry/Vec4_t.h"

Vec3_t::Vec3_t(double _x, double _y, double _z){
    x = _x;
    y = _y;
    z = _z;
}

Vec3_t::~Vec3_t()= default;

void Vec3_t::setValues(const double _x, const double _y, const double _z) {
    x = _x;
    y = _y;
    z = _z;
}

Vec3_t& Vec3_t::operator=(const Vec3_t& rhs) {
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
}

Vec3_t Vec3_t::operator +(const Vec3_t &other) const{
    return {x + other.x, y + other.y, z+other.z};
}

Vec3_t Vec3_t::operator -(const Vec3_t &other) const{
    return {x - other.x, y - other.y, z - other.z};
}

Vec3_t Vec3_t::operator *(const double factor) const{
    return {x * factor, y * factor, z * factor};
}

Vec3_t Vec3_t::operator /(const double factor) const{
    return {x / factor, y / factor, z / factor};
}

double Vec3_t::length() const{
    return sqrt( x * x + y * y + z * z );
}

Vec3_t Vec3_t::cross(const Vec3_t& b) const {
    return {y * b.z - b.y * z,
            z * b.x - b.z * x,
            x * b.y - b.x * y};
}

double Vec3_t::dot(const Vec3_t& b) const {
    return x * b.x + y * b.y + z * b.z;
}

Vec3_t Vec3_t::rotate_x(const double angle) const{
    return {x,
            y * cos(angle) - z * sin(angle),
            y*sin(angle) + z * cos(angle)};
}
Vec3_t Vec3_t::rotate_y(const double angle) const{
    return {x * cos(angle) + z * sin(angle),
            y,
            -x * sin(angle) + z  * cos(angle)};
}
Vec3_t Vec3_t::rotate_z(const double angle) const{
    return {x * cos(angle) - y * sin(angle),
            x * sin(angle) + y * cos(angle),
            z};
}

void Vec3_t::normalize(){
    if (const double factor = this->length(); factor != 0){
        x /= factor;
        y /= factor;
        z /= factor;
    }else{
        x = 0;
        y = 0;
        z = 0;
    }
}

double Vec3_t::getX() const{
    return x;
}

double Vec3_t::getY() const{
    return y;
}
double Vec3_t::getZ() const{
  return z;
}

Vec4_t Vec3_t::toVec4() const{
    return {x, y, z, 1};
}