#pragma once
#include <SDL2/SDL.h>

namespace window {
    void run();
    void quit();
    SDL_Window*& getWindow();
    SDL_Renderer*& getRenderer();
    int getW();
    int getH();
    const char* getTitle();
}
