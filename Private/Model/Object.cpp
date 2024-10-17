#include "../../Public/Model/Object.h"

Object::Object(const int _id, const std::string &_name, const std::string &_type, const std::string &_description, Model *_model)
{
    matrix = Mat4_t::identity();
    id = _id;
    name = _name;
    type = _type;
    description = _description;
    model = _model;
    group = _model->currentGroup;

    this->model->numberOfObjects++;
}

Object::~Object(){
    for(const Face * f : faces) {
        delete f;
    }
    getModel() -> objects.erase(this);

    model -> freedObjects.push(id);

    /*if(getModel() -> objects.empty()) {
        delete getModel();
    }*/

    group->objects.erase(this);

    model -> numberOfObjects --;
    /*if(group->objects.empty()) {
        delete group;
    }*/

}



int Object::getId() const {
    return id;
}

std::string Object::getName() const {
    return name;
}

std::string Object::getType() const {
    return type;
}

std::string Object::getDescription() const {
    return description;
}

Mat4_t Object::getMatrix() const {
    return matrix;
}

Model *Object::getModel() const {
    return model;
}

Group *Object::getGroup() const{
    return group;
}

void Object::setId(const int _id) {
    id = _id;
}

void Object::setName(const std::string &_name) {
    name = _name;
}

void Object::setType(const std::string &_type) {
    type = _type;
}

void Object::setDescription(const std::string &_description) {
    description = _description;
}

void Object::setFaces(const std::set<Face *> &_faces) {
    faces = _faces;
}

void Object::setModel(Model *_model) {
    model = _model;
}

void Object::setMatrix(const Mat4_t &_matrix){
    matrix = _matrix;
}


void Object::rotate(const double _x, const double _y, const double _z) {
    matrix = matrix * Mat4_t::make_rotate(_x, _y, _z);
}
void Object::translate(const double _x, const double _y, const double _z) {
    matrix = matrix * Mat4_t::make_translate(_x, _y, _z);
}
void Object::scale(const double _x, const double _y, const double _z) {
    matrix = matrix * Mat4_t::make_scale(_x, _y, _z);
}
void Object::rotateX(const double _x) {
    matrix = matrix * Mat4_t::make_rx(_x);
}
void Object::rotateY(const double _y) {
    matrix = matrix * Mat4_t::make_ry(_y);
}
void Object::rotateZ(const double _z) {
    matrix = matrix * Mat4_t::make_rz(_z);
}


void Object::getTreeHierarchy(ofstream &fileOut) const {
    fileOut << "\t\tId: "<<getId() << " ";
    fileOut << "\tName: "<<getName() << " ";
    fileOut << "\tType: "<<getType() << " ";
    fileOut << "\tDescription: "<<getDescription() << "\n";

    for(const Face *f : faces) {
        f->getTreeHierarchy(fileOut);
    }

    fileOut <<"\t\tFree Points:\n";
    for(const Point *p : points) {
        if(p->faces.empty()) {
            p->getTreeHierarchy(fileOut);
        }
    }
}
