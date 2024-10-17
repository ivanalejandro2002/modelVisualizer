#include "../../Public/Model/Model.h"

#include <thread>

Model::Model(const int id, const std::string &name, const std::string &description){
    this->id = id;
    this->name = name;
    this->description = description;

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

void Model::selectGroupById(const int id) {
    currentGroup = groupsById[id];
    currentObject = nullptr;
    currentFace = nullptr;
    currentPoint = nullptr;
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

void Model::selectObjectById(const int id) {
    currentObject = objectsById[id];
    currentGroup = currentObject->getGroup();
    currentFace = nullptr;
    currentPoint = nullptr;
}

void Model::selectObjectByObject(Object *obj) {
    currentObject = obj;
    currentGroup = currentObject->getGroup();
    currentFace = nullptr;
    currentPoint = nullptr;
}

void Model::selectPointById(const int id) {
    currentPoint = pointsById[id];
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
bool Model::selectFaceById(const int id) {
    if(!facesById.contains(id))return false;
    currentFace = facesById[id];
    currentObject = currentFace->getObject();
    currentGroup = currentObject->getGroup();
    return true;
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

void Model::renderPoints(int style, const Drawer *drawer,int fov) const {
    for(const auto point: points){
        Vec3_t p = (point->getObject()->getMatrix() * (point->toVec4())).toVec3();
        Vec2_t location = project(p,style);
        location = location*fov;
        location = location + Vec2_t(drawer->getWidth()/static_cast<double>(2),drawer->getHeight()/static_cast<double>(2));
        drawer->drawPoint(static_cast<int>(location.getX()),static_cast<int>(location.getY()));
    }
}

Vec2_t Model::project(Vec3_t &p, const int type) {
    //0 Isometric
    //1 Plano
    double prevX = p.getX();
    double prevY = p.getY();
    const double prevZ = p.getZ();
    if(type == 0){
        return {prevX,prevY};
    }else{
        double newX = prevX / prevZ;
        double newY = prevY / prevZ;
        return {newX,newY};
    }
}