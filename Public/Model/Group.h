#pragma once
#include "Model.h"
#include "Object.h"
#include <fstream>
class Model;
class Object;
class Group{
    private:
        int id;
        std::string name;
        std::string type;
        std::string description;
        Model *model;
    public:
        std::set<Object*> objects;


        Group(int,const std::string &name, const std::string &type, const std::string &description, Model *model);
        ~Group();
        void setId(int id);
        void setName(const std::string &name);
        void setType(const std::string &type);
        void setDescription(const std::string &description);
        void setModel(Model *model);

        [[nodiscard]] int getId() const;
        std::string getName();
        std::string getType();
        std::string getDescription();
        [[nodiscard]] Model *getModel() const;

        void getTreeHierarchy(std::ofstream &) const;
};