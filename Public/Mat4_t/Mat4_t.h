#pragma once

#include <vector>
#include "../Geometry/Vec4_t.h"
class Vec4_t;
class Mat4_t{
    private:

    public:
        std::vector<std::vector<double> > mat;
        Mat4_t();
        explicit Mat4_t(double mat[4][4]);
        Mat4_t(const Mat4_t& mat);

        ~Mat4_t();

        Mat4_t& operator=(const Mat4_t&);
        Mat4_t operator+(const Mat4_t&) const;
        Mat4_t operator-(const Mat4_t&) const;
        Mat4_t operator*(const Mat4_t&) const;
        Vec4_t operator*(const Vec4_t&) const;

        static Mat4_t identity();
        [[nodiscard]] Mat4_t transpose() const;

        static Mat4_t make_scale(double, double, double);
        static Mat4_t make_translate(double, double, double);
        static Mat4_t make_rotate(double, double, double);
        static Mat4_t make_rx(double);
        static Mat4_t make_ry(double);
        static Mat4_t make_rz(double);
};