#pragma once

#include "../Geometry/Vec3_t.h"
#include "../Geometry/Vec2_t.h"
#include "Face.h"
#include "Model.h"
#include <set>
#include <fstream>
class Face;
class Object;
class Vec3_t;
class Model;
class Point:public Vec3_t{
  private:
    int id;
    Model *model;
    Object *object;
  public:
    std::set <Face *> faces;  //Indicador de cuales son las caras correspondientes a un punto util para borrar puntos
    Point(double, double, double, int, Model *, Object *);
    ~Point();
    void destroy();

    [[nodiscard]] int getId() const;

    [[nodiscard]] Model *getModel() const;
    [[nodiscard]] Object *getObject() const;

    void setPosition(double x, double y, double z);
    void setID(int);

    void addFace(Face *face);
    void removeFace(Face *face);

    void removeAllFaces();
    [[nodiscard]] Vec3_t toVec3_t() const;

    void getPositions();

    void getTreeHierarchy(std::ofstream &) const;
};