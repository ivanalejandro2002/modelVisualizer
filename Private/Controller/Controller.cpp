#include "../../Public/Controller/Controller.h"

#include <SDL_events.h>

Controller::Controller() {
    running = true;
    fov = 400;
}

Controller::~Controller() = default;

bool Controller::getRunning() const {
    return running;
}
void Controller::setRunning(const bool running){
    Controller::running = running;
}


void Controller::process_input() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            running = false;
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
        if(event.key.keysym.sym == SDLK_i)
            typeOfView = 1;
        if(event.key.keysym.sym == SDLK_o)
            typeOfView = 0;
        break;
        default: ;
    }
}

int Controller::getFov() const {
    return fov;
}

int Controller::getTypeOfView() const {
    return typeOfView;
}

