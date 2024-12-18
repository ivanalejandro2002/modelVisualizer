#pragma once

#include <iostream>
#include <vector>
#include <SDL.h>
#include "../Geometry/Vec2_t.h"
#include "../Geometry/Vec3_t.h"
using namespace std;
class Drawer{
private:
    SDL_Window *screen;
    SDL_Renderer *renderer;
    int width, height;
    SDL_Color color{};
public:
    Drawer(int, int, const string&);
    ~Drawer();

    //Canvass oriented
    void changeBrushColor(SDL_Color color);
    void fillColor(SDL_Color color) const;
    void fillScreen() const;
    void updateScreen() const;


    //figure oriented
    static vector<pair<int,int> > bresenhamByX(int,int,int,int) ;
    static vector<pair<int,int> > bresenhamByY(int,int,int,int);
    static vector<pair<int,int> > bresenham(int,int,int,int);

    void createLineBresenham(Vec2_t , Vec2_t ) const;
    void createCheatLine(const Vec2_t&,const Vec2_t&) const;
    void drawPolygon(const vector<Vec2_t> &) const;
    void drawConnectedPolygon(const vector<Vec2_t > &) const;

    [[nodiscard]] int getHeight() const;
    [[nodiscard]] int getWidth() const;

    void drawPoint(int x, int y) const;

    void drawHorizontalLine(int,int,int);
    void drawHorizontalLineInterpolation(int,double,double,const Vec3_t &, const Vec3_t &, const Vec3_t &);
    void drawFilledTriangleInterpolation(vector<pair<Vec2_t,Vec3_t> >, const Vec3_t &);
    void drawFlatTopInterpolation(const vector<pair<Vec2_t,Vec3_t> >&, const Vec3_t &);
    void drawFLatBottomInterpolation(const vector<pair<Vec2_t,Vec3_t> > &,const Vec3_t &);

    void drawFilledTriangle(vector<Vec2_t> );
    void drawFlatTop(const vector<Vec2_t>& );
    void drawFLatBottom(const vector<Vec2_t> &);

    void drawCircle(int, int, int) const;
    void circleSymmetry(int,int,int,int) const;

    void drawEllipseMidPoint(int,int,int,int) const ;

    static int sgnd(const double);
    static Vec2_t intersectLinesD(const Vec2_t &, const Vec2_t &, const Vec2_t &, const Vec2_t &);
    int intersectLineSegmentInfo(const Vec2_t &,const Vec2_t &, const Vec2_t &, const Vec2_t &);
    vector<Vec2_t> polygonCut(const vector<Vec2_t> &,const Vec2_t &,const Vec2_t &);
    void drawScanLineFill(vector<Vec2_t>);
};