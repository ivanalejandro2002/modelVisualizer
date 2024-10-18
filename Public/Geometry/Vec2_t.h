#pragma once

#include <cmath>
class Vec2_t{
    private:
        double x;
        double y;
    public:
        const double eps = 1e-9;
        Vec2_t(double x, double y);
        ~Vec2_t();

        [[nodiscard]] double length() const;

        Vec2_t operator +(const Vec2_t &) const;
        Vec2_t operator -(const Vec2_t &) const;
        Vec2_t operator *(double) const;
        Vec2_t operator /(double) const;
        Vec2_t& operator =(const Vec2_t &);

        [[nodiscard]] double getX() const;
        [[nodiscard]] double getY() const;

        void setX(double);
        void setY(double);

        void normalize();
};