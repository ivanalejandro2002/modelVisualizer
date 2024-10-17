#pragma once

#include <set>
#include "Face.h"
#include "Model.h"
#include "Point.h"
#include <fstream>
#include "../Mat4_t/Mat4_t.h"
class Face;
class Vec3_t;
class Model;
class Group;
class Point;
class Object{
    private:
        int id;
        std::string name;
        std::string type;
        std::string description;
        Mat4_t matrix;
        Model* model;
        Group* group;
    public:
        std::set<Face *> faces;
        std::set<Point *> points;
        Object();
        Object(int, const std::string &name, const std::string &type, const std::string &description, Model* model);
        ~Object();

        [[nodiscard]] int getId() const;
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] std::string getType() const;
        [[nodiscard]] std::string getDescription() const;
        [[nodiscard]] Model* getModel() const;
        [[nodiscard]] Group* getGroup() const;
        std::vector<Face *> getFaces();
        [[nodiscard]] Mat4_t getMatrix() const;

        void setId(int id);
        void setName(const std::string &_name);
        void setType(const std::string &_type);
        void setDescription(const std::string &_description);
        void setPosition(const Vec3_t &_position);
        void setRotation(const Vec3_t &rotation);
        void setScale(const Vec3_t &scale);
        void setFaces(const std::set<Face *> &faces);
        void setModel(Model* _model);
        void setMatrix(const Mat4_t &);

        void rotate(double, double, double);
        void scale(double, double, double);
        void translate(double, double, double);
        void rotateX(double);
        void rotateY(double);
        void rotateZ(double);

        void getTreeHierarchy(std::ofstream &) const;
};