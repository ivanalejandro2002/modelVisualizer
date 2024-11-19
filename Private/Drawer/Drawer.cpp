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

Drawer::~Drawer() = default;

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

void Drawer::createCheatLine(const Vec2_t& p1, const Vec2_t& p2) const {
    SDL_RenderDrawLine(renderer,static_cast<int>(p1.getX()),
                                static_cast<int>(p1.getY()),
                                static_cast<int>(p2.getX()),
                                static_cast<int>(p2.getY()));
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
        drawHorizontalLine(i,max(static_cast<int>(xStart),0),min(static_cast<int>(xEnd),width));
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
        drawHorizontalLine(i,max(static_cast<int>(xStart),0),min(static_cast<int>(xEnd),width));
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

void Drawer::circleSymmetry(const int xc, const int yc, const int x, const int y) const {
    drawPoint(xc+x, yc+y);
    drawPoint(xc-x, yc+y);
    drawPoint(xc+x, yc-y);
    drawPoint(xc-x, yc-y);
    drawPoint(xc+y, yc+x);
    drawPoint(xc-y, yc+x);
    drawPoint(xc+y, yc-x);
    drawPoint(xc-y, yc-x);
}


void Drawer::drawCircle(const int xc, const int yc, const int r) const {
    int x = 0, y = r;
    int d = 3 - 2 * r;
    circleSymmetry(xc, yc, x, y);
    while (y >= x){

        // check for decision parameter
        // and correspondingly
        // update d, y
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
            d = d + 4 * x + 6;

        // Increment x after updating decision parameter
        x++;

        // Draw the circle using the new coordinates
        circleSymmetry(xc, yc, x, y);
    }
}

void Drawer::drawEllipseMidPoint(int rx, int ry , int xc, int yc) const {
    float x = 0;
    float y = ry;

    // Initial decision parameter of region 1
    float d1 = (ry * ry) - (rx * rx * ry) +
               (0.25 * rx * rx);
    float dx = 2 * ry * ry * x;
    float dy = 2 * rx * rx * y;

    // For region 1
    while (dx < dy)
    {

        // Print points based on 4-way symmetry
        drawPoint(x + xc,y + yc);
        drawPoint(-x + xc,y + yc);
        drawPoint(x + xc,-y + yc);
        drawPoint(-x + xc,-y + yc);


        // Checking and updating value of
        // decision parameter based on algorithm
        if (d1 < 0)
        {
            x++;
            dx = dx + (2 * ry * ry);
            d1 = d1 + dx + (ry * ry);
        }
        else
        {
            x++;
            y--;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d1 = d1 + dx - dy + (ry * ry);
        }
    }

    // Decision parameter of region 2
    float d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) +
               ((rx * rx) * ((y - 1) * (y - 1))) -
               (rx * rx * ry * ry);

    // Plotting points of region 2
    while (y >= 0)
    {

        // Print points based on 4-way symmetry
        drawPoint(x + xc,y + yc);
        drawPoint(-x + xc,y + yc);
        drawPoint(x + xc,-y + yc);
        drawPoint(-x + xc,-y + yc);

        // Checking and updating parameter
        // value based on algorithm
        if (d2 > 0)
        {
            y--;
            dy = dy - (2 * rx * rx);
            d2 = d2 + (rx * rx) - dy;
        }
        else
        {
            y--;
            x++;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d2 = d2 + dx - dy + (rx * rx);
        }
    }
}

int Drawer::sgnd(const double x) {
    if(x > 1e-8)return 1;
    if(-x > 1e-8)return -1;
    return 0;
}

Vec2_t Drawer::intersectLinesD(const Vec2_t &a1, const Vec2_t &v1, const Vec2_t &a2, const Vec2_t &v2) {
    const double det = v1.cross(v2);
    return a1 + v1 * ((a2 - a1).cross(v2) / det);
}


int Drawer::intersectLineSegmentInfo(const Vec2_t & a, const Vec2_t & v, const Vec2_t & c, const Vec2_t & d) {
    const Vec2_t v2 = d - c;
    const double det = v.cross(v2);
    double EPS = 1e-8;
    if(abs(det) <= EPS) {
        if(abs((c - a).cross(v))<=EPS) {
            return -1;
        }else {
            return 0;
        }
    }else {
        return sgnd(v.cross(c -a) != sgnd(v.cross(d-a)));
    }
}


vector<Vec2_t> Drawer::polygonCut(const vector<Vec2_t> & P, const Vec2_t & a,const  Vec2_t & v) {
    int n = static_cast<int>(P.size());

    vector<Vec2_t> lhs;
    double EPS = 1e-9;

    for(int i = 0; i < n; i++) {
        if(v.cross(P[i] - a) >= -EPS) {
            lhs.push_back(P[i]);
        }

        if(intersectLineSegmentInfo(a, v, P[i], P[(i + 1) % n]) == 1) {
            Vec2_t p = intersectLinesD(a, v, P[i], P[(i + 1) % n] - P[i]);
            if(p != P[i] && p != P[(i + 1) % n]) {
                lhs.push_back(p);
            }
        }
    }
    return lhs;
}


void Drawer::drawScanLineFill(vector<Vec2_t> points) {
    vector<pointGeom> temporal;
    for(const Vec2_t& p : points) {
        temporal.emplace_back(p.getX(), p.getY());
    }

    temporal = cutPolygon(temporal, {0,0},{0,-1});
    temporal = cutPolygon(temporal, {0,0},{1,0});
    temporal = cutPolygon(temporal, {static_cast<double>(width),static_cast<double>(height)},{0,1});
    temporal = cutPolygon(temporal, {static_cast<double>(width),static_cast<double>(height)},{-1,0});

    points.clear();
    for(auto p : temporal) {
        points.emplace_back(p.x,p.y);
    }

    int minY = INT_MAX;
    int maxY = INT_MIN;
    for(const Vec2_t& point: points) {
        minY = min(minY,static_cast<int>(point.getY()));
        maxY = max(maxY,static_cast<int>(point.getY()));
    }

    vector<pair<Vec2_t , Vec2_t> > edges;

    for(int i = 0; i < points.size(); i++) {
        Vec2_t p1 = points[i];
        Vec2_t p2 = points[(i+1)%points.size()];
        if(p1.getY() != p2.getY()) {
            if(p1.getY()>p2.getY())
                swap(p1,p2);

            edges.emplace_back(p1 , p2);
        }
    }
    for(int i = minY ; i<=maxY ; i++) {
        vector<int> intersections;
        for(const auto&[p1, p2] : edges) {
            if(p1.getY() <= i && i < p2.getY()) {
                const double dx = p2.getX() - p1.getX();
                const double dy = p2.getY() - p1.getY();
                const double x = p1.getX() + (i - p1.getY()) * (dx / dy);
                intersections.push_back(static_cast<int>(x));
            }
        }

        ranges::sort(intersections);
        for(int j = 0; j < intersections.size(); j += 2) {
            if(j+1 < intersections.size()) {
                drawHorizontalLine(i, intersections[j], intersections[j+1]);
            }
        }
    }
}
