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
    modelo.loadObj("../TestModels/tetera.obj");
    //modelo.getTreeHierarchy(fout);

    fout<<modelo.currentObject->getName()<<"\n";



    drawer->changeBrushColor({255,255,255,255});
    int fov = 400;
    char lectura;
    bool drawVertex = true;
    bool drawEdges = false;
    bool drawFaces = false;
    fout<<modelo.sizeOfFacesBuffer<<"\n";
    for(auto face: modelo.faces){
        //int rojo = rand()%200+30;
        //int verde = rand()%200+30;
        //int azul = rand()%200+30;
        int rojo = 230;
        int verde = 190;
        int azul = 103;

        face->setColor(rojo*1000000+verde*1000+azul);
    }
    double accumulative = 0;
    double aum = 0.05;
    while(controller->getRunning()) {
        int previousFrameTime = SDL_GetTicks();
        lectura = controller->process_input();
        if(lectura=='v'){
            drawVertex=!drawVertex;
            if(!drawVertex && !drawEdges && !drawFaces)
                drawVertex=!drawVertex;
        }
        if(lectura=='l'){
            drawEdges=!drawEdges;
            if(!drawVertex && !drawEdges && !drawFaces)
                drawEdges=!drawEdges;
        }
        if(lectura=='f'){
            drawFaces=!drawFaces;
            if(!drawVertex && !drawEdges && !drawFaces)
                drawFaces=!drawFaces;
        }
        if(lectura=='p') {
            controller->setTypeOfView((controller->getTypeOfView()+1)%3);
        }
        if(lectura=='w'){
            modelo.getCamera().rotateCamera(0.1,0,0);
        }
        if(lectura=='s'){
            modelo.getCamera().rotateCamera(-0.1,0,0);
        }

        //modelo.currentObject->translate(0,0,12);

        if(accumulative<-10){
            aum=+0.05;
        }else if(accumulative>10){
            aum = -0.05;
        }
        for(Object *obj : modelo.objects) { 
            //obj->rotate(0.01,0.01,0.00);

            //obj->translate(0,0,-accumulative);
            //obj->rotate(0.01,0.01,0.01);

            obj->translate(0.00,0.00,aum);

        }
        accumulative+=aum;
        modelo.currentObject->translate(0,0,0);
        Mat4_t aux = modelo.currentObject->getMatrix();

        drawer->changeBrushColor({248,122,83,255});
        if(drawVertex)
            modelo.renderPoints(controller->getTypeOfView(),drawer,controller->getFov());

        drawer->changeBrushColor({230,199,103,255});

        if(drawFaces)
            modelo.renderAllFilledFaces(controller->getTypeOfView(),drawer,controller->getFov());
        //modelo.renderFilledFaces(controller->getTypeOfView(),drawer,controller->getFov(),colorForFaces);

        drawer->changeBrushColor({76,75,22,255});
        if(drawEdges)
            modelo.renderFaces(controller->getTypeOfView(),drawer,controller->getFov());

        if(const int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks()-previousFrameTime); timeToWait >0 && timeToWait <= FRAME_TARGET_TIME){
            SDL_Delay(timeToWait);
        }
        drawer->changeBrushColor({0,0,255,255});
        modelo.renderCamera(controller->getTypeOfView(),drawer,controller->getFov());

        drawer->changeBrushColor({255,0,0,255});
        modelo.renderCameraVector(controller->getTypeOfView(),drawer,controller->getFov());

        //drawer->drawFilledTriangle({{10,500},{0,550},{200,500}});
        //drawer->drawCircle(100,100,100);
        //drawer->drawEllipseMidPoint(100,50,100,100);
        //drawer->changeBrushColor({0,255,0,10});
        //drawer->drawScanLineFill({{10,10},{20,10},{60,60},{10,20}});
        //drawer->drawScanLineFill({{-1000,-1000},{1000,-1000},{1000,1000},{-1000,1000}});
        drawer->updateScreen();
        drawer->fillColor({0,0,0,255});

        modelo.unmarkPoints();
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
