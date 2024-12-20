#include "../../Public/Model/Model.h"
#include <algorithm>
#include <thread>
#include <cassert>

Model::Model(const int id, const std::string &name, const std::string &description){
    this->id = id;
    this->name = name;
    this->description = description;

    camera.setPosition({0,0,0});
    camera.setTarget({0,0,0});

    numberOfFaces = 0;
    numberOfObjects = 0;
    numberOfPoints = 0;
    numberOfGroups = 0;

    sizeOfFacesBuffer = 0;
    sizeOfObjectBuffer = 0;
    sizeOfPointBuffer = 0;
    sizeOfGroupBuffer = 0;

    currentFace = nullptr;
    currentPoint = nullptr;
    currentObject = nullptr;
    currentGroup = nullptr;
}

Model::~Model() {
    for(const Face *face: this->faces) {
        delete face;
    }
}

int Model::getId() const {
    return this->id;
}

std::string Model::getName() const {
    return this->name;
}

std::string Model::getDescription() const {
    return this->description;
}

std::string Model::getType() const {
    return this->type;
}

Camera Model::getCamera() const{
    return camera;
}

void Model::setId(const int id) {
    this->id = id;
}
void Model::setName(const std::string &name) {
    this->name = name;
}
void Model::setDescription(const std::string &description) {
    this->description = description;
}
void Model::setType(const std::string &type) {
    this->type = type;
}

void Model::addFace(Face *face) {
    this->faces.insert(face);

    int newID;

    if(!this -> freedFaces.empty()) {
        newID = freedFaces.top();
        freedFaces.pop();
    }else {
        newID = sizeOfFacesBuffer++;
    }

    numberOfFaces ++;

    face -> setId(newID);
}

void Model::addObject(Object *object) {
    this->objects.insert(object);
    int newID;
    if(!this -> freedObjects.empty()) {
        newID = freedObjects.top();
        freedObjects.pop();
    }else {
        newID = sizeOfObjectBuffer++;
    }

    numberOfObjects++;

    object -> setId(newID);
}

void Model::addPoint(Point *point) {
    this->points.insert(point);

    int newID;

    if(!this -> freedPoints.empty()) {
        newID = freedPoints.top();
        freedPoints.pop();
    }else {
        newID = sizeOfPointBuffer++;
    }

    numberOfPoints++;

    point->setID(newID);
}

void Model::removeFace() {
    if(currentFace == nullptr) {
        return;
    }
    currentFace->destroy();
    currentFace = nullptr;
    currentPoint = nullptr;
}

void Model::removeObject() {
    delete currentObject;
    currentObject = nullptr;
    currentFace = nullptr;
    currentPoint = nullptr;
}

void Model::removePoint() {
    currentPoint->destroy();
    currentPoint = nullptr;
}

void Model::removeGroup() {
    delete currentGroup;
    currentGroup = nullptr;
    currentObject = nullptr;
    currentFace = nullptr;
    currentPoint = nullptr;
}

int Model::createGroup(const string &groupName, const string &groupType, const string &groupDescription) {
    if(groupsByName.contains(groupName)) {
        //error
        return -1;
    }
    int newID;
    if(this -> freedGroups.empty()) {
        newID = sizeOfGroupBuffer++;
    }else {
        newID = freedGroups.top();
        freedGroups.pop();
    }
    auto *newGroup = new Group(newID, groupName, groupType, groupDescription, this);

    this->groups.insert(newGroup);

    currentGroup = newGroup;
    currentObject = nullptr;
    currentFace = nullptr;
    currentPoint = nullptr;

    groupsByName[groupName] = newGroup;
    groupsById[newID] = newGroup;
    return 1;
}

int Model::createObject(const string &objectName, const string &objectType, const string &objectDescription) {
    if(objectsByName.contains(objectName)) {
        return -1;
    }
    if(currentGroup == nullptr) {
        return -1;
    }
    int newID;
    if(this -> freedObjects.empty()) {
        newID = sizeOfObjectBuffer++;
    }else {
        newID = freedObjects.top();
        freedObjects.pop();
    }
    auto *newObject = new Object(newID, objectName, objectType, objectDescription, this);
    this->objects.insert(newObject);
    currentGroup -> objects.insert(newObject);

    objectsById[newID] = newObject;
    objectsByName[objectName] = newObject;

    currentObject = newObject;
    currentFace = nullptr;
    currentPoint = nullptr;
    return 1;
}

int Model::createPoint(const Vec3_t& point) {
    if(currentObject == nullptr) {
        return -1;
    }
    int newID;
    if(this -> freedPoints.empty()) {
        newID = sizeOfPointBuffer++;
    }else {
        newID = freedPoints.top();
        freedPoints.pop();
    }
    auto *newPoint = new Point(point.getX(),point.getY(),point.getZ(),newID, this, currentObject);

    this->points.insert(newPoint);
    pointsById[newID] = newPoint;
    currentObject -> points.insert(newPoint);

    currentPoint = newPoint;
    currentFace = nullptr;

    return newID;
}

int Model::meshToFace(const vector <int> &pts) {
    if(currentObject == nullptr) {
        return -1;
    }

    bool possible = true;

    vector<Point*> newPoints;

    for(int p:pts) {
        if(!currentObject->points.contains(pointsById[p])) {
            possible = false;
            break;
        }else {
            newPoints.push_back(pointsById[p]);
        }
    }
    if(!possible) {
        return -1;
    }

    int newID;
    if(this -> freedFaces.empty()) {
        newID = sizeOfFacesBuffer ++;
    }else{
        newID = freedFaces.top();
        freedFaces.pop();
    }
    const auto newFace = new Face(newID,newPoints,this,currentObject);

    for(Point *p:newPoints) {
        p->faces.insert(newFace);
    }

    this->faces.insert(newFace);
    facesById[newID] = newFace;
    currentObject->faces.insert(newFace);
    selectFaceByFace(newFace);

    return newID;
}

int Model::createFace(const vector<Vec3_t> & _points) {
    vector<int> newPoints;
    for(const Vec3_t& pto : _points) {
        newPoints.push_back(createPoint(pto) );
    }

    return this->meshToFace(newPoints);

}


void Model::selectGroupByName(const std::string &name) {
    currentGroup = groupsByName[name];
    currentObject = nullptr;
    currentFace = nullptr;
    currentPoint = nullptr;
}

bool Model::selectGroupById(const int id) {
    if(!groupsById.contains(id))return false;
    currentGroup = groupsById[id];
    currentObject = nullptr;
    currentFace = nullptr;
    currentPoint = nullptr;
    return true;
}

void Model::selectGroupByGroup(Group *group) {
    currentGroup = group;
    currentObject = nullptr;
    currentFace = nullptr;
    currentPoint = nullptr;
}

void Model::selectObjectByName(const std::string &name) {

    currentObject = objectsByName[name];
    currentGroup = currentObject->getGroup();
    currentFace = nullptr;
    currentPoint = nullptr;
}

bool Model::selectObjectById(const int _id) {
    if(!objectsById.contains(_id))return false;
    currentObject = objectsById[_id];
    currentGroup = currentObject->getGroup();
    currentFace = nullptr;
    currentPoint = nullptr;
    return true;
}

void Model::selectObjectByObject(Object *obj) {
    currentObject = obj;
    currentGroup = currentObject->getGroup();
    currentFace = nullptr;
    currentPoint = nullptr;
}

void Model::selectPointById(const int _id) {
    currentPoint = pointsById[_id];
    currentObject = currentPoint->getObject();
    currentGroup = currentObject->getGroup();
    currentFace = nullptr;
}
void Model::selectPointByPoint(Point* point) {
    currentPoint = point;
    currentObject = currentPoint->getObject();
    currentGroup = currentObject->getGroup();
    currentFace = nullptr;
}

void Model::selectFaceByFace(Face* f) {
    currentFace = f;
    currentObject = currentFace->getObject();
    currentGroup = currentObject->getGroup();
    currentPoint = nullptr;
}
bool Model::selectFaceById(const int _id) {
    if(!facesById.contains(_id))return false;
    currentFace = facesById[_id];
    currentObject = currentFace->getObject();
    currentGroup = currentObject->getGroup();
    return true;
}

Point * Model::getPointById(const int _id) {
    if(!pointsById.contains(_id))return nullptr;
    return pointsById[_id];
}




void Model::getTreeHierarchy(ofstream &fileOut) const {
    fileOut << "Id: "<<id<<"\n";
    fileOut << "Name: "<<name<<"\n";
    fileOut << "Description: "<<description<<"\n";
    fileOut << "Type: "<<type<<"\n";
    fileOut << "NumberOfFaces: "<<numberOfFaces<<"\n";
    fileOut << "NumberOfObjects: "<<numberOfObjects<<"\n";
    fileOut << "NumberOfPoints: "<<numberOfPoints<<"\n";

    for(Group * group : this->groups) {
        group -> getTreeHierarchy(fileOut);
    }
}

void Model::renderPoints(const int style, const Drawer *drawer, const int fov){
    double EPS = 0;
    if(style == 2)EPS = 1;
    for(const auto object : this->objects) {
        Mat4_t transition = object->getMatrix();
        if(style==1){
            transition = Mat4_t::isometric()*transition;
        }else if(style==2) {
            //vec3_t nuevo =
            transition = camera.getAntiDirectionalMatrix() * transition;
        }
        for(const auto face: object->faces) {
            for(const auto point: face->getVertices()){
                Vec3_t p = {0,0,0};
                Vec2_t location = {0,0};

                if(!point->getUpdated()) {

                    p = (transition * (point->toVec4())).toVec3();
                    p = projectTo3DSimplified(p, style);
                    point->setUpdated(true);
                    point->setNewCoordinates(p);

                    location = {p.getX(),p.getY()};
                    location = location * fov;
                    location = location + Vec2_t(drawer->getWidth() / static_cast<double>(2),
                                                 drawer->getHeight() / static_cast<double>(2));
                    point->setScreenCoords(location);

                    if (p.getZ() <= EPS)continue;
                }else{
                    if(point->getNewCoordinates().getZ()<=EPS)continue;
                    location = point->getScreenCoords();
                }
                drawer->drawPoint(static_cast<int>(location.getX()),static_cast<int>(location.getY()));
            }
        }
    }
}

void Model::renderFaces(const int style, const Drawer *drawer, const int fov) {
    for(const auto object: objects) {
        Mat4_t transition = object->getMatrix();

        if(style==1){
            transition = Mat4_t::isometric()*transition;
        }else if(style==2) {
            //vec3_t nuevo =
            transition = camera.getAntiDirectionalMatrix() * transition;
        }


        for(const auto face: object->faces){
            vector<Point *> points = face->getVertices();
            vector<Vec2_t> locations;
            vector<double> zCoords;
            Vec3_t p = {0,0,0};
            Vec2_t location = {0,0};
            for(Point* point:face->getVertices()) {
                if(!point->getUpdated()) {

                    p = (transition * (point->toVec4())).toVec3();
                    p = projectTo3DSimplified(p, style);
                    point->setNewCoordinates(p);
                    zCoords.push_back(p.getZ());
                    location = {p.getX(),p.getY()};
                    location = location*fov;
                    location = location + Vec2_t(drawer->getWidth()/static_cast<double>(2),drawer->getHeight()/static_cast<double>(2));
                    point->setScreenCoords(location);
                    point->setUpdated(true);
                }else {
                    p = point->getNewCoordinates();
                    location = point->getScreenCoords();
                    zCoords.push_back(p.getZ());
                }
                locations.push_back(location);
            }
            Vec2_t newA(0,0),newB(0,0),delta(0,0);
            double EPS = 1e-5;
            for(int i = 0 ;i < locations.size(); i++) {
                //drawer->createLineBresenham(locations[i],locations[(i+1)%locations.size()]);
                const double A = zCoords[i];
                const double B = zCoords[(i + 1) % locations.size()];
                newA = locations[i];
                newB = locations[(i+1)%locations.size()];
                if(style==2) EPS = 1;
                if(A<=EPS && B <=EPS)continue;
                if(A>EPS && B<=EPS) {
                    delta = (newB - newA);
                    delta = delta / (A-B+EPS);
                    newB = newB + delta*B;
                }

                if(A<=EPS && B>EPS) {
                    delta = newA - newB;
                    delta = delta / (B-A+EPS);
                    newA = newA + delta*A;
                }
                drawer->createCheatLine(newA,newB);

            }
        }
    }
}

void Model::renderFilledFaces(const int style, Drawer *drawer, const int fov) const {
    for(const auto face: faces){
        vector<Point *> points = face->getVertices();
        vector<Vec2_t> locations;
        for(const Point *point:points) {
            Vec3_t p = (point->getObject()->getMatrix() * (point->toVec4())).toVec3();
            Vec2_t location = project(p,style);
            location = location*fov;
            location = location + Vec2_t(drawer->getWidth()/static_cast<double>(2),drawer->getHeight()/static_cast<double>(2));

            locations.push_back(location);
        }

        drawer->drawFilledTriangle(locations);
    }
}

void Model::renderFilledFaces(const int style, Drawer *drawer, const int fov, const vector<int> &colors) {
    vector< pair<double,pair< vector<Vec2_t> , int > > > rangedFaces;
    int index = 0;
    for(const auto face: faces){
        vector<Point *> points = face->getVertices();
        vector<Vec2_t> locations;
        double realZ = 0;
        for(const auto *point:points) {

            Vec3_t p = (point->getObject()->getMatrix() * (point->toVec4())).toVec3();
            Vec3_t locationX = projectTo3D(p,style);
            Vec2_t location = {locationX.getX(),locationX.getY()};
            realZ += locationX.getZ();
            location = location*fov;
            location = location + Vec2_t(drawer->getWidth()/static_cast<double>(2),drawer->getHeight()/static_cast<double>(2));

            locations.push_back(location);
        }

        double distance = realZ / static_cast<double>(points.size());
        rangedFaces.push_back({distance,{locations,face->getId()}});
        ++index;
    }

    ranges::sort(rangedFaces);
    for(int i = static_cast<int>(rangedFaces.size())-1;i >= 0; --i){
        const Uint8 red = colors[rangedFaces[i].second.second] / 1000000;
        const Uint8 green = (colors[rangedFaces[i].second.second] / 1000) % 1000;
        const Uint8 blue = rangedFaces[i].second.second % 1000;
        drawer->changeBrushColor({red,green,blue,255});
        drawer->drawFilledTriangle(rangedFaces[i].second.first);
    }

}

void Model::renderAllFilledFaces(const int style, Drawer *drawer, const int fov,bool shaders) {
    vector< pair<double,pair< vector<Vec2_t> , pair<int,vector<Vec3_t> > > > > rangedFaces;
    int index = 0;
    double EPS  = 0;
    if(style == 2)EPS = 1;
    for(const auto *group: groups) {
        for(const auto *object:group->objects) {
            Mat4_t transition = object->getMatrix();

            if(style==1){
                transition = Mat4_t::isometric()*transition;
            }else if(style==2) {
                //vec3_t nuevo =
                transition = camera.getAntiDirectionalMatrix() * transition;
            }

            for(const auto face: object->faces) {
                vector<Point *> points = face->getVertices();
                vector<Vec2_t> locations;
                vector<Vec3_t> locations3D;
                double realZ = 0;
                double realY = 0;
                double realX = 0;
                bool atLeastOne = false;
                bool allAlong = true;
                Vec3_t p = {0,0,0};
                Vec3_t locationX = {0,0,0};
                Vec2_t location = {0,0};
                for(auto *point:points) {
                    if(!point->getUpdated()) {

                        //point->setUpdated(true);
                        p = (transition * (point->toVec4())).toVec3();
                        locationX = projectTo3DSimplified(p,style);

                        point->setNewCoordinates(locationX);

                        location = {locationX.getX(),locationX.getY()};
                        realZ += locationX.getZ();
                        realY += locationX.getY();
                        realX += locationX.getX();
                        location = location*fov;
                        location = location + Vec2_t(drawer->getWidth()/static_cast<double>(2),drawer->getHeight()/static_cast<double>(2));

                        point->setScreenCoords(location);
                    }else {
                        locationX = point -> getNewCoordinates();
                        realZ += locationX.getZ();
                        realY += locationX.getY();
                        realX += locationX.getX();
                        location = point -> getScreenCoords();
                    }

                    locations.push_back(location);
                    locations3D.push_back(locationX);

                    if(locationX.getZ()>EPS)atLeastOne = true;
                    if(locationX.getZ()<=EPS)allAlong = false;
                }
                if(style==2 && !allAlong)continue;
                if(!atLeastOne)continue;

                if(!isVisibleVec(locations3D))continue;

                double distance = (Vec3_t(realX , realY , realZ) / static_cast<double>(points.size()) - camera.getPosition()).length();
                rangedFaces.push_back({distance,{locations,{face->getColor(),locations3D}}});
                ++index;
            }
        }
    }

    ranges::sort(rangedFaces);
    bool allAlong = true;
    for(int i = static_cast<int>(rangedFaces.size())-1;i >= 0; --i){
        double lightFactor = flatShadingFactor({0,0,-1},{rangedFaces[i].second.second.second});
        //lightFactor = 1;
        if(!shaders)lightFactor = 1;
        const Uint8 red = (rangedFaces[i].second.second.first / 1000000)*lightFactor;
        const Uint8 green = ((rangedFaces[i].second.second.first / 1000) % 1000)*lightFactor;
        const Uint8 blue = (rangedFaces[i].second.second.first % 1000)*lightFactor;
        drawer->changeBrushColor({red,green,blue,255});

        for(const Vec2_t& point: rangedFaces[i].second.first) {
            if(point.getX() < 0 || point.getY() < 0 || point.getX() > drawer->getWidth() || point.getY() > drawer->getHeight())allAlong = false;
        }

        if(allAlong)
            drawer->drawFilledTriangle(rangedFaces[i].second.first);
        else {
            drawer->drawScanLineFill(rangedFaces[i].second.first);
        }
    }
}

void Model::renderCamera(const int style, const Drawer *drawer, const int fov) {
    Vec3_t p = camera.getPosition();
    Vec2_t location = project(p,style);
    location = location*fov;
    location = location + Vec2_t(drawer->getWidth()/static_cast<double>(2),drawer->getHeight()/static_cast<double>(2));
    drawer->drawPoint(static_cast<int>(location.getX()),static_cast<int>(location.getY()));
}

void Model::renderCameraVector(const int style, const Drawer *drawer, const int fov) {
    Vec3_t p = camera.getPosition();
    Vec3_t dir = {0,0,1};
    dir = (camera.getDirectionMatrix() * dir.toVec4()).toVec3();
    Vec2_t location = project(dir,style);
    location = location*fov;
    location = location + Vec2_t(drawer->getWidth()/static_cast<double>(2),drawer->getHeight()/static_cast<double>(2));
    drawer->drawPoint(static_cast<int>(location.getX()),static_cast<int>(location.getY()));

    drawer->createCheatLine({drawer->getWidth()/static_cast<double>(2),drawer->getHeight()/static_cast<double>(2)},location);
}


Vec2_t Model::project(const Vec3_t &p, const int type) {
    //0 Isometric
    //1 Plano
    double prevX = p.getX();
    const double prevY = p.getY();
    if(type == 0){
        return {prevX,-prevY};
    }else if(type==1){
        const Vec3_t nuevo = (Mat4_t::isometric()*p.toVec4()).toVec3();
        return {nuevo.getX(),-nuevo.getY()};
    }else if(type==2) {
        if(p.getZ()==0) {
            return {p.getX(),p.getY()};
        }
        return {prevX/p.getZ(),-prevY/p.getZ()};
    }
    return {p.getX(),-p.getY()};
}

Vec2_t Model::projectSimplified(const Vec3_t &p, const int type) {
    //0 Isometric
    //1 Plano
    double prevX = p.getX();
    const double prevY = p.getY();
    if(type == 0){
        return {prevX,-prevY};
    }else if(type==1){
        return {prevX,-prevY};
    }else if(type==2) {
        if(p.getZ()==0) {
            return {prevX,-prevY};
        }
        return {prevX/p.getZ(),-prevY/p.getZ()};
    }
    return {prevX,-prevY};
}

Vec3_t Model::projectTo3D(const Vec3_t &p, const int type) {
    //0 Isometric
    //1 Plano
    double prevX = p.getX();
    double prevY = p.getY();
    double prevZ = p.getZ();
    if(type == 0){
        return {prevX,-prevY,prevZ};
    }else if(type==1){
        Vec3_t nuevo = (Mat4_t::isometric()*p.toVec4()).toVec3();
        return {nuevo.getX(),-nuevo.getY(),prevZ};
    }else if(type==2) {
        //vec3_t nuevo =
        const Vec3_t nuevo = (camera.getAntiDirectionalMatrix() * p.toVec4()).toVec3();
        if(nuevo.getZ() == 0) {
            return nuevo;
        }
        return {nuevo.getX()/nuevo.getZ(),-nuevo.getY()/nuevo.getZ(),prevZ};
    }
    return {prevX,-prevY,prevZ};
}

Vec3_t Model::projectTo3DSimplified(const Vec3_t &p, int type) {
    double prevX = p.getX();
    double prevY = p.getY();
    double prevZ = p.getZ();
    if(type == 0){
        return {prevX,-prevY,prevZ};
    }else if(type==1){
        return {prevX,-prevY,prevZ};
    }else if(type==2) {
        //vec3_t nuevo =
        if(prevZ == 0) {
            return {prevX,-prevY,prevZ};
        }
        return {prevX/prevZ,-prevY/prevZ,prevZ};
    }
    return {prevX,-prevY,prevZ};
}

void getNumAutomaton(const string &input,int &index, int &value) {
    while(index<input.size() && input[index]>='0' && input[index]<='9') {
        value *= 10;
        value += input[index] - '0';
        ++index;
    }
}

tuple <int,int,int> Model::readFacePoint(const string &input,int &index) {
    int t,n;
    while(index<input.length() && input[index] == ' ')index++;
    int v = n = t = -1;
    if(index<input.length() && input[index] != '/') {
        v = 0;
        getNumAutomaton(input,index,v);
    }
    if(index<input.length() && input[index]=='/')index++;
    if(index<input.length() && input[index] != ' ' && input[index] != '/') {
        t = 0;
        getNumAutomaton(input,index,t);
    }
    if(index<input.length() && input[index]=='/')index++;

    if(index<input.length() && input[index]!=' ' && input[index] != '/') {
        n = 0;
        getNumAutomaton(input,index,n);
    }

    return {v,t,n};
}

void Model::loadObj(const string &inputFile) {
    ifstream file;
    ofstream fout;
    fout.open("Log.txt");
    file.open(inputFile.c_str());
    if(!file.is_open()) {
        return;
    }
    map<int,int> realPoints;
    map<int,int> realFaces;
    string lecture;
    bool usedGroup = false;
    bool usedObject = false;
    int groupID;
    int objectID;
    int numberOfPoints = 0;
    if(!this->freedGroups.empty()) {
        groupID = this->freedGroups.top();
    }else {
        groupID = this->sizeOfGroupBuffer;
    }

    if(!this->freedObjects.empty()) {
        groupID = this->freedObjects.top();
    }else {
        groupID = this->sizeOfObjectBuffer;
    }

    string numberG = to_string(groupID);
    string numberO = to_string(objectID);
    this->createGroup("Loaded group ID = "+numberG,"Import","Imported Group from an OBJ file");
    this->createObject("Loaded object ID = "+numberO,"Import","Imported Object from an OBJ file");
    while(file>>lecture) {
        int idx = 1;
        if(lecture.empty()) continue;
        if(lecture[0] == 'g') {
            getline(file,lecture);
            idx = 0;
            while(lecture[idx] == ' ')idx++;
            string newName;

            while(idx < lecture.size() && lecture[idx]!=' ') {
                newName += lecture[idx];
                idx++;
            }

            if(!usedGroup) {
                usedGroup = true;
                this->currentGroup->setName(newName);
            }else {
                this->createGroup(newName,"Import","Imported Group from an OBJ file");

                if(!this->freedObjects.empty()) {
                    numberO = this->freedObjects.top();
                }else {
                    numberO = this->sizeOfObjectBuffer;
                }

                this->createObject("Loaded object ID = "+numberO,"Import","Imported Object from an OBJ file");
                usedObject = false;
            }
        }else if(lecture[0] == 'o') {
            getline(file,lecture);
            idx = 0;
            while(lecture[idx] == ' ')idx++;
            string newName;

            while(idx < lecture.size() && lecture[idx]!=' ') {
                newName += lecture[idx];
                idx++;
            }

            if(!usedObject) {
                usedObject = true;
                this->currentObject->setName(newName);
            }else {
                this->createObject(newName,"Import","Imported Group from an OBJ file");
            }
        }else if(lecture == "v") {
            double x,y,z;
            file>>x>>y>>z;

            int PointID = this -> createPoint(Vec3_t(x,y,z));
            numberOfPoints++;
            realPoints[numberOfPoints] = PointID;
            //fout<<"---"<<PointID<<"\n";
        }else if(lecture=="f") {
            vector<int> facePoints;
            vector<int> texturePoints;
            vector<int> normalPoints;

            getline(file,lecture);
            int index = 0;
            bool ended = false;
            while(!ended) {
                //fout<<":)\n";
                auto [vv,vt,vn] = readFacePoint(lecture,index);
                //fout<<vv<<" "<<vt<<" "<<vn<<"\n";
                if(vv == -1) {
                    ended = true;
                    continue;
                }

                facePoints.push_back(realPoints[vv]);
                texturePoints.push_back(realPoints[vt]);
                normalPoints.push_back(realPoints[vn]);


            }
            //fout<<sizeOfPointBuffer<<"\n";
            if(this->meshToFace(facePoints)<0) {
                fout<<"Error meshing : {";
                for(auto p: facePoints) {
                    fout<<p<<", ";
                }
                fout<<"} from group: "<<currentGroup->getName()<<"."<<currentObject->getName()<<"\n";
            }
        }else {
            getline(file,lecture);
        }
    }
}

bool Model::isVisible(const Face &face) {
    double component = 0;
    const vector<Point *> vertexes = face.getVertices();

    const Vec3_t A = vertexes[2]->toVec3_t() - vertexes[0]->toVec3_t();
    const Vec3_t B = vertexes[1]->toVec3_t() - vertexes[0]->toVec3_t();
    const Vec3_t cameraRay = camera.getPosition() - vertexes[0]->toVec3_t();

    const Vec3_t normal = A.cross(B);

    component = normal.dot(cameraRay);
    return component>-1e-1;
}

bool Model::isVisibleVec(const vector<Vec3_t> &vertexes) {
    double component = 0;

    Vec3_t A = vertexes[2] - vertexes[0];
    Vec3_t B = vertexes[1] - vertexes[0];
    //A.normalize();
    //B.normalize();
    const Vec3_t cameraRay = camera.getPosition() - vertexes[0];

    const Vec3_t normal = A.cross(B);

    component = normal.dot(cameraRay);
    return component>-2.5e-2;
}

void Model::unmarkPoints() {
    for(const auto p: points){
        p->setUpdated(false);
    }
}


double Model::flatShadingFactor(const Vec3_t &light, const vector<Vec3_t> &figure) {
    Vec3_t A = figure[1] - figure[0];
    Vec3_t B = figure[2] - figure[0];

    Vec3_t C = A.cross(B);
    C.normalize();

    double w = -C.dot(light);
    w = max(w,static_cast<double>(0));
    w = min(w,static_cast<double>(1));
    return w;
}

Vec3_t Model::normalSum(const Point *p, const Face *original) {
    Vec3_t normal = Vec3_t(0,0,0);
    for(Face *f: p->faces) {
        if(f == original) continue;

        Vec3_t A = f->getVertices()[1]->toVec3_t() - f->getVertices()[0]->toVec3_t();
        Vec3_t B = f->getVertices()[2]->toVec3_t() - f->getVertices()[0]->toVec3_t();

        Vec3_t C = A.cross(B);

        normal = normal + C;
    }
    normal.normalize();
    return normal;
}


void Model::renderAllFilledFacesGouraud(const int style, Drawer *drawer, const int fov,bool shaders,double ka, double kd,const Vec3_t& la, const Vec3_t& ld, int lightFactor, const Vec3_t & lightOrigin) {
    vector< pair<double,pair< vector<Vec2_t> , pair<int,vector<Vec3_t> > > > > rangedFaces;
    vector<pair<double, Face * > >referencedFaces;
    int index = 0;
    double EPS  = 0;
    if(style == 2)EPS = 1;
    for(const auto *group: groups) {
        for(const auto *object:group->objects) {
            Mat4_t transition = object->getMatrix();

            if(style==1){
                transition = Mat4_t::isometric()*transition;
            }else if(style==2) {
                //vec3_t nuevo =
                transition = camera.getAntiDirectionalMatrix() * transition;
            }

            for(const auto face: object->faces) {
                vector<Point *> points = face->getVertices();
                vector<Vec2_t> locations;
                vector<Vec3_t> locations3D;
                double realZ = 0;
                double realY = 0;
                double realX = 0;
                bool atLeastOne = false;
                bool allAlong = true;
                Vec3_t p = {0,0,0};
                Vec3_t locationX = {0,0,0};
                Vec2_t location = {0,0};
                for(auto *point:points) {
                    if(!point->getUpdated()) {

                        //point->setUpdated(true);
                        p = (transition * (point->toVec4())).toVec3();
                        locationX = projectTo3DSimplified(p,style);

                        point->setNewCoordinates(locationX);

                        location = {locationX.getX(),locationX.getY()};
                        realZ += locationX.getZ();
                        realY += locationX.getY();
                        realX += locationX.getX();
                        location = location*fov;
                        location = location + Vec2_t(drawer->getWidth()/static_cast<double>(2),drawer->getHeight()/static_cast<double>(2));

                        point->setScreenCoords(location);
                    }else {
                        locationX = point -> getNewCoordinates();
                        realZ += locationX.getZ();
                        realY += locationX.getY();
                        realX += locationX.getX();
                        location = point -> getScreenCoords();
                    }

                    locations.push_back(location);
                    locations3D.push_back(locationX);

                    if(locationX.getZ()>EPS)atLeastOne = true;
                    if(locationX.getZ()<=EPS)allAlong = false;
                }
                if(style==2 && !allAlong)continue;
                if(!atLeastOne)continue;

                if(!isVisibleVec(locations3D))continue;

                double distance = (Vec3_t(realX , realY , realZ) / static_cast<double>(points.size()) - camera.getPosition()).length();
                rangedFaces.push_back({distance,{locations,{face->getColor(),locations3D}}});
                referencedFaces.emplace_back(distance,face);
                ++index;
            }
        }
    }

    ranges::sort(rangedFaces);
    ranges::sort(referencedFaces);
    for(int i = static_cast<int>(rangedFaces.size())-1;i >= 0; --i) {

        /*for(const Vec2_t& point: rangedFaces[i].second.first) {
            if(point.getX() < 0 || point.getY() < 0 || point.getX() > drawer->getWidth() || point.getY() > drawer->getHeight())allAlong = false;
        }*/

        vector<Vec3_t> normals;
        for(int j=0;j<3;j++) {
            normals.emplace_back(0,0,0);
        }

        vector<Vec3_t> shading;
        for(int j=0;j<3;j++) {
            shading.emplace_back(la*ka);
        }
        vector<pair<Vec2_t,Vec3_t> > projectionData;
        //-------Defused lightning------//
        normals[0] = normalSum(referencedFaces[i].second->getVertices()[0],referencedFaces[i].second);
        normals[1] = normalSum(referencedFaces[i].second->getVertices()[1],referencedFaces[i].second);
        normals[2] = normalSum(referencedFaces[i].second->getVertices()[2],referencedFaces[i].second);
        normals[0].normalize();
        normals[1].normalize();
        normals[2].normalize();

        Vec3_t L = lightOrigin - rangedFaces[i].second.second.second[0];
        double d = L.length();
        L.normalize();
        shading[0] = shading[0] + ld*max(L.dot(normals[0])*kd,static_cast<double>(0));


        L = lightOrigin - rangedFaces[i].second.second.second[1];
        d = L.length();
        L.normalize();
        shading[1] = shading[1] + ld*max(L.dot(normals[1])*kd,static_cast<double>(0));

        L = lightOrigin - rangedFaces[i].second.second.second[2];
        d = L.length();
        L.normalize();
        shading[2] = shading[2] + ld*max(L.dot(normals[2])*kd,static_cast<double>(0));

        //---------Specular Lightning-----------
        L = lightOrigin - rangedFaces[i].second.second.second[0];
        d = L.length();
        L.normalize();
        Vec3_t R = normals[0] *(2 * (normals[0].dot(L))) - L;
        shading[0] = shading[0] + ld*max(pow(R.dot(normals[0]),lightFactor)*kd,static_cast<double>(0));

        L = lightOrigin - rangedFaces[i].second.second.second[1];
        d = L.length();
        L.normalize();
        R = normals[1] *(2 * (normals[1].dot(L))) - L;
        shading[1] = shading[1] + ld*max(pow(R.dot(normals[1]),lightFactor)*kd,static_cast<double>(0));

        L = lightOrigin - rangedFaces[i].second.second.second[2];
        d = L.length();
        L.normalize();
        R = normals[2] *(2 * (normals[2].dot(L))) - L;
        shading[2] = shading[2] + ld*max(pow(R.dot(normals[2]),lightFactor)*kd,static_cast<double>(0));

        projectionData.clear();
        for(int idx=0;idx<3;++idx) {
            projectionData.emplace_back(rangedFaces[i].second.first[idx],shading[idx]);
        }
        drawer->drawFilledTriangleInterpolation(projectionData,{static_cast<double>(rangedFaces[i].second.second.first / static_cast<double>(1000000)),static_cast<double>((rangedFaces[i].second.second.first / 1000) % 1000),static_cast<double>(rangedFaces[i].second.second.first % 1000)});
        //drawer->drawFilledTriangleInterpolation(projectionData,{255,255,255});
        //drawer->drawFilledTriangle(rangedFaces[i].second.first);
    }
}