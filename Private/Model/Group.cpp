#include "../../Public/Model/Group.h"

Group::Group(const int id, const std::string &name, const std::string &type, const std::string &description, Model *_model){
    this->id = id;
    this->name = name;
    this->type = type;
    this->description = description;
    this->model = _model;
}
Group::~Group(){

    for(const Object *obj : this->objects) {
        delete obj;
    }
    this->model->groups.erase(this);

    model->numberOfGroups --;

    /*if(this->model->groups.empty()) {
        delete this->model;
    }*/
}

int Group::getId() const {
    return this->id;
}

std::string Group::getName() {
    return this->name;
}

std::string Group::getType() {
    return this->type;
}

std::string Group::getDescription() {
    return this->description;
}

Model *Group::getModel() const {
    return this->model;
}

void Group::setId(const int id) {
    this->id = id;
}

void Group::setName(const std::string &name) {
    this->name = name;
}

void Group::setType(const std::string &type) {
    this->type = type;
}

void Group::setDescription(const std::string &description) {
    this->description = description;
}

void Group::setModel(Model *model) {
    this->model = model;
}

void Group::getTreeHierarchy(ofstream &fileOut) const {
    fileOut << "\tid: " << this->id << endl;
    fileOut << "\tname: " << this->name << endl;
    fileOut << "\ttype: " << this->type << endl;
    fileOut << "\tdescription: " << this->description << endl;
    fileOut << "\tmodel: " << this->model << endl;
    fileOut << "\tGroup Code: " << this << endl;

    for(const Object *obj : this->objects) {
        obj->getTreeHierarchy(fileOut);
    }
}
