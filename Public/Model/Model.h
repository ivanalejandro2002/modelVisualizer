#pragma once

#include <vector>
#include <map>
#include <stack>
#include <tuple>
#include "Object.h"
#include "../Geometry/Vec3_t.h"
#include "Face.h"
#include "Group.h"
#include "Camera.h"
#include "../Drawer/Drawer.h"
using namespace std;
class Face;
class Object;
class Point;
class Group;
class Model{
    private:
        int id;
        std::string name;
        std::string type;
        std::string description;
        Camera camera;

    public:
        set<Object *> objects;
        set<Point *> points;
        set<Face *> faces;
        set<Group *> groups;

        stack<int> freedObjects;
        stack<int> freedFaces;
        stack<int> freedPoints;
        stack<int> freedGroups;

        map<string,Object*> objectsByName;
        map<string,Group*> groupsByName;

        map<int,Object*> objectsById;
        map<int,Point*> pointsById;
        map<int,Face*> facesById;
        map<int,Group*> groupsById;

        Object * currentObject;
        Point * currentPoint;
        Face * currentFace;
        Group * currentGroup;

        int numberOfObjects;
        int numberOfFaces;
        int numberOfPoints;
        int numberOfGroups;

        int sizeOfObjectBuffer;
        int sizeOfFacesBuffer;
        int sizeOfPointBuffer;
        int sizeOfGroupBuffer;

        Model();
        Model(int id, const std::string &name, const std::string &description);
        ~Model();

        void setName(const std::string &name);
        void setDescription(const std::string &description);
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] std::string getDescription() const;
        [[nodiscard]] Camera getCamera() const;

        void setId(int id);
        [[nodiscard]] int getId() const;

        [[nodiscard]] std::string getType() const;
        void setType(const std::string &type);


        void addPoint(Point *point);
        void addFace(Face *face);
        void addObject(Object *object);

        void removeObject(int id);
        void removeObject();
        void removeObject(Object *object);
        void removeFace(int id);
        void removeFace();
        void removeFace(Face *face);
        void removePoint(int id);
        void removePoint();
        void removePoint(Point *point);
        void removeGroup(int id);
        void removeGroup(Group *group);
        void removeGroup();

        int createGroup(const string &groupName,const string &groupType, const string &groupDescription);
        int createObject(const string &objectName,const string &objectType, const string &objectDescription);
        int createPoint(const Vec3_t& point);
        int meshToFace(const vector<int> &);
        int createFace(const vector<Vec3_t>&);

        void selectGroupByName(const std::string &name);
        bool selectGroupById(int id);
        void selectGroupByGroup(Group *);

        void selectObjectByName(const std::string &name);
        bool selectObjectById(int id);
        void selectObjectByObject(Object *);

        bool selectFaceById(int id);
        void selectFaceByFace(Face *);

        void selectPointById(int id);
        void selectPointByPoint(Point *);

        Point * getPointById(int id);

        void getTreeHierarchy(ofstream &) const;

        void renderPoints(int, const Drawer *,int) const;
        void renderFilledFaces(int, Drawer *, int) const;
        void renderFilledFaces(int, Drawer *, int, const vector<int> &);
        void renderAllFilledFaces(int, Drawer *, int, const vector<int> &);
        void renderFaces(int, const Drawer *,int) const;

        void renderCamera(int,const Drawer *,int);
        void renderCameraVector(int,const Drawer *,int);

        [[nodiscard]] static Vec2_t project(const Vec3_t &,int type) ;
        Vec3_t projectTo3D(const Vec3_t &,int type) ;
        Vec3_t projectTo3DSimplified(const Vec3_t &, int type);

        void loadObj(const string &);

        static tuple<int,int,int> readFacePoint(const string &,int &);


        bool isVisible(const Face &);
        bool isVisibleVec(const vector<Vec3_t> &);

        void unmarkPoints();
};