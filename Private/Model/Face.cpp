#include "../../Public/Model/Face.h"

Face::Face(const int id, const vector<Point *> &vertices, Model *_model, Object *_object){
    this->id = id;
    this->vertices = vertices;
    this->object = _object;
    this->model = _model;

    this->model->numberOfFaces ++;
}

Face::~Face() = default;

void Face::destroy() {
    for (const auto & vertex : vertices) {
        vertex -> faces.erase(this);
        /*if(vertex -> faces.empty()) {
            delete vertex;
            vertex = nullptr;
        }*/
    }

    getObject() -> faces.erase(this);
    model -> facesById.erase(id);

    model -> freedFaces.push(id);

    model -> numberOfFaces --;

    /*if(getObject() -> points.empty()) {
        delete getObject();
    }*/

    delete this;
}
int Face::getId() const {
    return id;
}

vector<Point *> Face::getVertices() const {
    return vertices;
}

Object *Face::getObject() const {
    return object;
}

void Face::setId(int _id) {
    id = _id;
}

void Face::getTreeHierarchy(ofstream &fileOut) const {
    fileOut << "\t\t\tId: "<<getId() << " ";
    fileOut << "\tVertices:\n";
    for (auto & vert : vertices) {
        vert -> getTreeHierarchy(fileOut);
    }
}
