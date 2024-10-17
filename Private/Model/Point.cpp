#include "../../Public/Model/Point.h"

Point::Point(const double _x, const double _y, const double _z, const int id, Model * _model, Object * _object): Vec3_t(_x, _y, _z) {
    this->id = id;
    this->model = _model;
    this->object = _object;

    this->model -> numberOfPoints ++;
}

Point::~Point() = default;
void Point::destroy() {
    for(const auto f:faces){
        f->destroy();
    }
    model -> freedPoints.push(id);
    model -> numberOfPoints --;

    object -> points.erase(this);
    /*if(object -> points.empty()) {
        delete object;
    }*/

    delete this;
}

int Point::getId() const {
    return id;
}

Model * Point::getModel() const {
    return model;
}

Object * Point::getObject() const {
    return object;
}

void Point::setPosition(double _x, double _y, double _z) {
    setValues(_x, _y, _z);
}

void Point::setID(const int _id) {
    id = _id;
}

void Point::addFace(Face *face) {
    faces.insert(face);
}

void Point::removeFace(Face *face) {
    if(faces.contains(face)) {
        faces.erase(face);
    } else {
        std::cerr << "Face does not exist" << std::endl;
    }
    if(faces.empty()) {
        delete this;
    }
}

void Point::removeAllFaces() {
    faces.clear();
    delete this;
}

Vec3_t Point::toVec3_t() const {
    return {getX() ,getY() ,getZ()};
}

void Point::getTreeHierarchy(ofstream &fileOut) const{
    fileOut << "\t\t\t\tId: " << this->id;
    fileOut << "\tX: " << this->getX();
    fileOut << "\tY: " << this->getY();
    fileOut << "\tZ: " << this->getZ();
    fileOut << "\n";
}
