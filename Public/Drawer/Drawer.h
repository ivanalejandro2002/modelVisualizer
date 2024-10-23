#pragma once

#include <iostream>
#include <vector>
#include <SDL.h>
#include "..\Geometry/Vec2_t.h"
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
    void createCheatLine(Vec2_t,Vec2_t) const;
    void drawPolygon(const vector<Vec2_t> &) const;
    void drawConnectedPolygon(const vector<Vec2_t > &) const;

    [[nodiscard]] int getHeight() const;
    [[nodiscard]] int getWidth() const;

    void drawPoint(int x, int y) const;
};