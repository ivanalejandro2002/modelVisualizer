#include "..\..\Public\Drawer\Drawer.h"

Drawer::Drawer(int _width, int _height, const string& title) {
    width = _width;
    height = _height;

    screen = SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(screen,-1, 0);

    color = {0,0,0,255};
    fillScreen();
}

Drawer::~Drawer() {}

vector<pair<int,int> > Drawer::bresenhamByX(int x1, int y1, int x2, int y2) {

    vector<pair<int, int> > points;
    if (x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
    }


    const int m_new = 2 * (y2 - y1);
    int realM = abs(m_new);
    int slope_error_new = m_new - (x2 - x1);
    for (int x = x1, y = y1; x <= x2; x++) {
        points.emplace_back(x,y);

        // Add slope to increment angle formed
        slope_error_new += realM;

        // Slope error reached limit, time to
        // increment y and update slope error.
        if (slope_error_new >= 0) {
            if(m_new>=0) {
                y++;
            }else{
                y--;
            }
            slope_error_new -= 2 * (x2 - x1);
        }
    }
    return points;
}

vector<pair<int,int> > Drawer::bresenhamByY(int x1, int y1, int x2, int y2){
    vector<pair<int,int> > points;
    if(y1>y2){
        swap(x1,x2);
        swap(y1,y2);
    }
    int m_new = 2 * (x2 - x1);
    int realM = abs(m_new);
    int slope_error_new = m_new - (y2 - y1);
    for (int y = y1, x = x1; y <= y2; y++) {
        points.emplace_back(x,y);

        // Add slope to increment angle formed
        slope_error_new += realM;

        // Slope error reached limit, time to
        // increment y and update slope error.
        if (slope_error_new >= 0) {
            if(m_new>=0) {
                x++;
            }else{
                x--;
            }
            slope_error_new -= 2 * (y2 - y1);
        }
    }
    return points;
}

vector<pair<int,int> > Drawer::bresenham(int x1, int y1, int x2, int y2){
    if(abs(x2-x1)>abs(y2-y1)){
        return bresenhamByX(x1,y1,x2,y2);
    }else{
        return bresenhamByY(x1,y1,x2,y2);
    }
}

void Drawer::createLineBresenham(Vec2_t p1, Vec2_t p2) const{
    vector<pair<int,int> > puntosLinea;
    puntosLinea = bresenham(static_cast<int>(round(p1.getX())),
                            static_cast<int>(round(p1.getY())),
                            static_cast<int>(round(p2.getX())),
                            static_cast<int>(round(p2.getY())));
    for(pair<int,int> z:puntosLinea) {
        SDL_RenderDrawPoint(renderer, z.first, z.second);
    }
}

void Drawer::drawPolygon(const vector<Vec2_t > &polygon) const{
    for(int i=0;i<polygon.size();i++){
        createLineBresenham(polygon[i],polygon[(i+1)%polygon.size()]);
    }
}

void Drawer::drawConnectedPolygon(const vector<Vec2_t > &polygon) const{
    for(int i=0;i<polygon.size();i++){
        for(int j=i+1;j<polygon.size();j++){
            createLineBresenham(polygon[i],polygon[j]);
        }
    }
}

void Drawer::changeBrushColor(SDL_Color _color){
    color = _color;
    SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
}

void Drawer::fillScreen() const{
    SDL_RenderClear(renderer);
}

void Drawer::fillColor(SDL_Color _color) const{
    SDL_SetRenderDrawColor(renderer,_color.r,_color.g,_color.b,_color.a);
    fillScreen();
    SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
}

void Drawer::updateScreen() const{
    SDL_RenderPresent(renderer);
}

int Drawer::getHeight() const {
    return height;
}

int Drawer::getWidth() const {
    return width;
}

void Drawer::drawPoint(const int x, const int y) const {
    SDL_Rect rectangle;
    rectangle.x =x-1;
    rectangle.y = y-1;
    rectangle.w = rectangle.h = 2;

    SDL_RenderFillRect(renderer,&rectangle);
}
