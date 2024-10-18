#include <iostream>
#include <vector>
#include <SDL.h>
#include <fstream>
#include "Public/Drawer/Drawer.h"
#include "Public/Model/Model.h"
#include "Public/Controller/Controller.h"

#define FRAMES_PER_SECOND 60
#define FRAME_TARGET_TIME (1000/FRAMES_PER_SECOND)
using namespace std;
ofstream fout;

struct point{
    double x;
    double y;
    double z;
    double p;
};
struct face;

struct object{
    vector<face *> faces;
};
struct face{
    vector<point *> vertexes;
};

vector<point> allVertexes;

int main(int argv, char ** args) {
    auto* drawer = new Drawer(1000,600,"Ventana");
    auto* controller = new Controller();
    fout.open("Lag.txt");


    /*Model modelo(1,"Modelito","Prueba de modelo");

    modelo.createGroup("Grupo1","Grupito","Prueba de grupos");
    modelo.createGroup("Grupo2","Grupito","Prueba de grupos2");

    modelo.createObject("Objeto1","Cubo","Prueba de objeto1");

    modelo.selectGroupByName("Grupo1");

    modelo.createObject("Objeto2","Cubo","Prueba de objeto2");

    modelo.createPoint({1,2,3});
    modelo.createPoint({4,5,6});
    modelo.createPoint({7,8,9});

    modelo.meshToFace({0,1,2});
    modelo.selectObjectByName("Objeto1");

    modelo.createFace({{1.5,2,0.5},{4,5,6},{7,8,9}});

    modelo.removeFace();
    modelo.meshToFace({3,4,5});

    modelo.selectPointById(4);
    modelo.removePoint();

    modelo.selectObjectByName("Objeto2");
    modelo.createPoint({-1,-2,-3});
    modelo.meshToFace({0,2,4});

    fout<<modelo.selectFaceById(0)<<"\n";
    modelo.removeFace();
    modelo.createFace({{0,0,0},{1,20,30},{1,1,1}});

    modelo.getTreeHierarchy(fout);*/

    Model modelo(1,"Cubito","Práctica 1 de Imagenes");
    modelo.loadObj("D:/Users/Ale/Documents/Documentos Alex/Alex/ESCOM/septimo semestre/imagenes/tetera.obj");
    modelo.getTreeHierarchy(fout);



    drawer->changeBrushColor({255,255,255,255});
    int fov = 400;

    while(controller->getRunning()) {
        int previousFrameTime = SDL_GetTicks();
        controller->process_input();

        //modelo.currentObject->translate(0,0,12);
        modelo.currentObject->rotate(0.01,0.01,0.01);
        modelo.currentObject->translate(0,0,0);
        Mat4_t aux = modelo.currentObject->getMatrix();


        modelo.renderPoints(controller->getTypeOfView(),drawer,controller->getFov());
        modelo.renderFaces(controller->getTypeOfView(),drawer,controller->getFov());

        if(const int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks()-previousFrameTime); timeToWait >0 && timeToWait <= FRAME_TARGET_TIME){
            SDL_Delay(timeToWait);
        }

        drawer->updateScreen();
        drawer->fillColor({0,0,0,255});
    }
    fout.close();



    /*const vector<Vec2_t > polygon ={
            {100, 250},
            {122, 321},
            {456, 222},
            {100,500},
            {90,400}
    };

    //drawer->drawConnectedPolygon(polygon);

    drawer->updateScreen();
    SDL_Delay(3000);
    SDL_Quit();*/
    return 0;
}
