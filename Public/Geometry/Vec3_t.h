#pragma once

#include <cmath>
#include "Vec4_t.h"
class Vec4_t;
class Vec3_t{
private:
    double x;
    double y;
    double z;
public:
    const double eps = 1e-9;
    Vec3_t(double, double, double);
    ~Vec3_t();

    void setValues(double, double, double);

    [[nodiscard]] double length() const;

    Vec3_t& operator =(const Vec3_t &);
    Vec3_t operator +(const Vec3_t &) const;
    Vec3_t operator -(const Vec3_t &) const;
    Vec3_t operator *(double) const;
    Vec3_t operator /(double) const;

    Vec3_t cross(const Vec3_t& b) const;
    double dot(const Vec3_t& b) const;

    [[nodiscard]] Vec3_t rotate_x(double) const;
    [[nodiscard]] Vec3_t rotate_y(double) const;
    [[nodiscard]] Vec3_t rotate_z(double) const;

    [[nodiscard]] double getX() const;
    [[nodiscard]] double getY() const;
    [[nodiscard]] double getZ() const;

    void normalize();
    [[nodiscard]] Vec4_t toVec4() const;
};