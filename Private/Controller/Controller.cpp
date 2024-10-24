#include "../../Public/Controller/Controller.h"

#include <SDL_events.h>

Controller::Controller() {
    running = true;
    fov = 10;
}

Controller::~Controller() = default;

bool Controller::getRunning() const {
    return running;
}
void Controller::setRunning(const bool running){
    Controller::running = running;
}


char Controller::process_input() {
    SDL_Event event;
    SDL_PollEvent(&event);
    char returnValue = 0;
    switch (event.type) {
        case SDL_QUIT:
            running = false;
            returnValue = -1;
        break;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE)
                running = false;
        if(event.key.keysym.sym == SDLK_UP)
            fov++;
        if(event.key.keysym.sym == SDLK_DOWN)
            fov--;
        if(event.key.keysym.sym == SDLK_LEFT)
            fov+=10;
        if(event.key.keysym.sym == SDLK_RIGHT)
            fov-=10;
        if(event.key.keysym.sym == SDLK_i) {
            typeOfView = 1;
            returnValue = 'i';
        }
        if(event.key.keysym.sym == SDLK_o) {
            typeOfView = 0;
            returnValue = 'o';
        }
        if(event.key.keysym.sym == SDLK_l)
            returnValue = 'l';
        if(event.key.keysym.sym == SDLK_f)
            returnValue = 'f';
        if(event.key.keysym.sym == SDLK_v)
            returnValue = 'v';

        break;
        default: ;
    }
    return returnValue;
}

int Controller::getFov() const {
    return fov;
}

int Controller::getTypeOfView() const {
    return typeOfView;
}

