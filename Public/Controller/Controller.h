#pragma once
#include <cstdint>

class Controller {
    private:
        bool running;
        int fov;
        int typeOfView;
    public:
        Controller();
        ~Controller();

        [[nodiscard]] bool getRunning() const;
        void setRunning(bool);
        [[nodiscard]] int getFov() const;
        [[nodiscard]] int getTypeOfView() const;

        char process_input();
};
