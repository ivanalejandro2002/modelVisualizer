#include "..\..\Public\Drawer\Drawer.h"
#include "pointGeom.cpp"
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
    if(p1.getX()<0 && p2.getX()<0)return;
    if(p1.getY()<0 && p2.getY()<0)return;
    if(p1.getX()>width && p2.getY()>width)return;
    if(p1.getY()>height && p2.getY()>height)return;
    bool exists = false;
    if(p1.getX()>=0 && p1.getX()<=width && p1.getY()>=0 && p1.getY()<=height)exists = true;
    if(p2.getX()>=0 && p2.getX()<=width && p2.getY()>=0 && p2.getY()<=height)exists = true;

    int w = intersectSegmentsInfo({p1.getX(),p1.getY()},{p2.getX(),p2.getY()},{0,0},{0,static_cast<ld>(height)});
    if(w==1){
        exists = true;
        const pointGeom v = intersectLines({p1.getX(),p1.getY()},{p2.getX()-p1.getX(),p2.getY()-p1.getY()},{0,0},{0,1});

        if(p1.getX() < p2.getX()) {
            p1.setX(v.x);
            p1.setY(v.y);
        }else {
            p2.setX(v.x);
            p2.setY(v.y);
        }
    }else if(w==-1) {
        exists = true;
        double miny = min(p1.getY(),p2.getY());
        miny = max(miny,static_cast<double>(0));
        double maxy = max(p1.getY(),p2.getY());
        maxy = min(static_cast<double>(height),maxy);
        p1.setY(miny);
        p2.setY(maxy);
    }

    w = intersectSegmentsInfo({p1.getX(),p1.getY()},{p2.getX(),p2.getY()},{static_cast<ld>(width),0},{static_cast<ld>(width),static_cast<ld>(height)});
    if(w==1){
        exists = true;
        const pointGeom v = intersectLines({p1.getX(),p1.getY()},{p2.getX()-p1.getX(),p2.getY()-p1.getY()},{static_cast<ld>(width),0},{0,1});

        if(p1.getX() > p2.getX()) {
            p1.setX(v.x);
            p1.setY(v.y);
        }else {
            p2.setX(v.x);
            p2.setY(v.y);
        }
    }else if(w==-1) {
        exists = true;
        double miny = min(p1.getY(),p2.getY());
        miny = max(miny,static_cast<double>(0));
        double maxy = max(p1.getY(),p2.getY());
        maxy = min(static_cast<double>(height),maxy);
        p1.setY(miny);
        p2.setY(maxy);
    }

    w = intersectSegmentsInfo({p1.getX(),p1.getY()},{p2.getX(),p2.getY()},{0,0},{static_cast<ld>(width),0});
    if(w==1){
        exists = true;
        const pointGeom v = intersectLines({p1.getX(),p1.getY()},{p2.getX()-p1.getX(),p2.getY()-p1.getY()},{0,0},{1,0});

        if(p1.getY() < p2.getY()) {
            p1.setX(v.x);
            p1.setY(v.y);
        }else {
            p2.setX(v.x);
            p2.setY(v.y);
        }
    }else if(w==-1) {
        exists = true;
        double minx = min(p1.getX(),p2.getX());
        minx = max(minx,static_cast<double>(0));
        double maxx = max(p1.getX(),p2.getX());
        maxx = min(static_cast<double>(width),maxx);
        p1.setY(minx);
        p2.setY(maxx);
    }

    w = intersectSegmentsInfo({p1.getX(),p1.getY()},{p2.getX(),p2.getY()},{0,static_cast<ld>(height)},{static_cast<ld>(width),static_cast<ld>(height)});
    if(w==1){
        exists = true;
        const pointGeom v = intersectLines({p1.getX(),p1.getY()},{p2.getX()-p1.getX(),p2.getY()-p1.getY()},{0,static_cast<ld>(height)},{1,0});

        if(p1.getY() > p2.getY()) {
            p1.setX(v.x);
            p1.setY(v.y);
        }else {
            p2.setX(v.x);
            p2.setY(v.y);
        }
    }else if(w==-1) {
        exists = true;
        double minx = min(p1.getX(),p2.getX());
        minx = max(minx,static_cast<double>(0));
        double maxx = max(p1.getX(),p2.getX());
        maxx = min(static_cast<double>(width),maxx);
        p1.setY(minx);
        p2.setY(maxx);
    }

    if(!exists)return;

    puntosLinea = bresenham(static_cast<int>(round(p1.getX())),
                            static_cast<int>(round(p1.getY())),
                            static_cast<int>(round(p2.getX())),
                            static_cast<int>(round(p2.getY())));
    for(pair<int,int> z:puntosLinea) {
        SDL_RenderDrawPoint(renderer, z.first, z.second);
    }
}

void Drawer::createCheatLine(Vec2_t p1, Vec2_t p2) const {
    SDL_RenderDrawLine(renderer,p1.getX(),p1.getY(),p2.getX(),p2.getY());
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
    rectangle.w = rectangle.h = 3;

    SDL_RenderFillRect(renderer,&rectangle);
}

void Drawer::drawHorizontalLine(int y, int x1, int x2){
    if(x1>x2)swap(x1,x2);
    for(int i=x1;i<=x2;i++){
        SDL_RenderDrawPoint(renderer,i,y);
    }
}

void Drawer::drawFilledTriangle(vector<Vec2_t> pts) {
    sort(pts.begin(),pts.end());
    if(pts[0].getY() == pts[2].getY()){
        drawHorizontalLine(pts[0].getY(),pts[0].getX(),pts[2].getX());
    }
    double x,y;
    y = pts[1].getY();
    x = ((pts[2].getX()-pts[0].getX())*(pts[1].getY()-pts[0].getY()))/(pts[2].getY()-pts[0].getY())+pts[0].getX();

    Vec2_t M(x,y);


    if(pts[1].getY()!=pts[2].getY() && pts[2].getY()!=y){
        drawFlatTop({pts[1],M,pts[2]});
    }
    if(pts[0].getY()!=pts[1].getY() && pts[0].getY()!=y){
        drawFLatBottom({pts[0],pts[1],M});
    }
}

void Drawer::drawFlatTop(const vector<Vec2_t> &pts) {
    double deltaX,deltaY;
    deltaX = pts[0].getX()-pts[2].getX();
    deltaY = pts[2].getY()-pts[0].getY();
    double m1 = deltaX/deltaY;

    deltaX = pts[1].getX()-pts[2].getX();
    deltaY = pts[2].getY()-pts[0].getY();

    double m2 = deltaX/deltaY;
    double xStart, xEnd;
    double origin;
    origin = xStart = xEnd = pts[2].getX();

    int startY = (int)pts[2].getY();
    int endY = (int)pts[0].getY();
    for(int i=startY;i>=endY;i--){
        drawHorizontalLine(i,(int)xStart,(int)xEnd);
        xStart+=m1;
        if(abs(pts[0].getX()-origin)<abs(xStart-origin)){
            xStart = pts[0].getX();
        }
        xEnd+=m2;
        if(abs(pts[1].getX()-origin)<abs(xEnd-origin)){
            xEnd = pts[1].getX();
        }
    }
}

void Drawer::drawFLatBottom(const vector<Vec2_t> &pts) {
    double deltaX,deltaY;
    deltaX = pts[0].getX()-pts[2].getX();
    deltaY = pts[0].getY()-pts[2].getY();
    if(deltaY==0)return;
    double m1 = deltaX/deltaY;

    deltaX = pts[1].getX()-pts[0].getX();
    deltaY = pts[2].getY()-pts[0].getY();
    if(deltaY==0)return;

    double m2 = deltaX/deltaY;
    double xStart, xEnd,origin;
    origin = xStart = xEnd = pts[0].getX();

    int startY = (int)pts[0].getY();
    int endY = (int)pts[2].getY();
    for(int i=startY;i<=endY;i++){
        drawHorizontalLine(i,(int)xStart,(int)xEnd);
        xStart+=m1;
        if(abs(pts[2].getX()-origin)<abs(xStart-origin)){
            xStart = pts[2].getX();
        }
        xEnd+=m2;
        if(abs(pts[1].getX()-origin)<abs(xEnd-origin)){
            xEnd = pts[1].getX();
        }
    }
}