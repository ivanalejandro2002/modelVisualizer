#pragma once

#include <cmath>
#include "Vec3_t.h"
#include <vector>
class Vec3_t;
class Vec4_t{
private:
    double x;
    double y;
    double z;
    double w;
public:
    const double eps = 1e-9;
    Vec4_t(double, double, double, double);
    explicit Vec4_t(const std::vector<double>&);
    ~Vec4_t();

    [[nodiscard]] double length() const;

    Vec4_t operator +(const Vec4_t &) const;
    Vec4_t operator -(const Vec4_t &) const;
    Vec4_t operator *(double) const;
    Vec4_t operator /(double) const;

    [[nodiscard]] double getX() const;
    [[nodiscard]] double getY() const;
    [[nodiscard]] double getZ() const;
    [[nodiscard]] double getW() const;

    [[nodiscard]] Vec3_t toVec3() const;
};