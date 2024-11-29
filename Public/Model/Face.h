#pragma once

#include "Point.h"
#include "Object.h"
#include <vector>
#include "Model.h"
#include <fstream>
using namespace std;
class Point;
class Object;
class Model;
class Face{
    private:
        int id;
        Model * model;
        vector<Point *> vertices;
        Object * object;
        int color;

    public:
        Face(int id, const vector<Point *> &, Model * model, Object * object);
        ~Face();

        void destroy();

        [[nodiscard]] int getId() const;
        [[nodiscard]] vector<Point *> getVertices() const;
        [[nodiscard]] Object * getObject() const;

        void setId(int id);
        void setColor(int);
        [[nodiscard]] int getColor() const;

        void getTreeHierarchy(ofstream &) const;
};