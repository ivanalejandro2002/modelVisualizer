#include "../../Public/Geometry/Vec2_t.h"

Vec2_t::Vec2_t(double _x, double _y){
    x = _x;
    y = _y;
}

Vec2_t::~Vec2_t()= default;

Vec2_t Vec2_t::operator +(const Vec2_t &other) const{
    return {x + other.x, y + other.y};
}

Vec2_t Vec2_t::operator -(const Vec2_t &other) const{
    return {x - other.x, y - other.y};
}

Vec2_t Vec2_t::operator *(const double factor) const{
    return {x * factor, y * factor};
}

Vec2_t Vec2_t::operator /(const double factor) const{
    return {x / factor, y / factor};
}

bool Vec2_t::operator <(const Vec2_t &other) const{
    if(y<other.y)return true;
    if(y>other.y)return false;
    return x<other.x;
}

Vec2_t& Vec2_t::operator=(const Vec2_t& rhs) {
    x = rhs.x;
    y = rhs.y;
    return *this;
}

bool Vec2_t::operator==(const Vec2_t &p) const {
    return abs(x - p.x) <=1e-8 && abs(y - p.y) <=1e-8;
}

bool Vec2_t::operator!=(const Vec2_t &p) const {
    return !(*this == p);
}


double Vec2_t::length() const{
    return sqrt( x * x + y * y);
}

void Vec2_t::normalize(){
    double factor = this->length();
    if (factor != 0){
        x /= factor;
        y /= factor;
    }else{
        x = 0;
        y = 0;
    }
}

double Vec2_t::getX() const{
    return x;
}

double Vec2_t::getY() const{
    return y;
}

void Vec2_t::setX(const double _x) {
    x = _x;
}
void Vec2_t::setY(const double _y) {
    y = _y;
}

double Vec2_t::cross(const Vec2_t & p) const {
    return x * p.y - y * p.x;
}
