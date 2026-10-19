#include "window.h"
#include "api.h"
#include "listeners.h"
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_keycode.h>
#include <SDL_mouse.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <SDL_video.h>
#include <iostream>
#include <utility>
std::pair<bool, const char*> init();

namespace window {
    void run(){
        auto i = init();
        if (!i.first){
            std::cerr << i.second << "\n";
            return;
        }
        api::image img("assets/images/blue-square.jpg");
        api::font f("assets/fonts/Montserrat-BoldItalic.ttf", 32);
        api::text t(f, "Hello");
        SDL_Event e;
        bool running=true;
        while (running){
            while (SDL_PollEvent(&e)) {
                switch (e.type) {
                    case SDL_QUIT:
                        running=false;
                        break;
                    case SDL_KEYDOWN:
                    case SDL_KEYUP:
                        keylistener::reg(e.key);
                        break;
                    case SDL_MOUSEMOTION:
                        mouselistener::reg(e.motion);
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                    case SDL_MOUSEBUTTONUP:
                        mouselistener::reg(e.button);
                        break;
                    case SDL_MOUSEWHEEL:
                        mouselistener::reg(e.wheel);
                        break;

                }
            }

            if (keylistener::isPressed(SDLK_ESCAPE)){
                running=false;
            }

            SDL_RenderClear(getRenderer());
            img.draw();
            t.draw();
            SDL_RenderPresent(getRenderer());

            keylistener::update();
            mouselistener::update();
            SDL_Delay(16);
        }
    }
    void quit(){
        SDL_DestroyWindow(getWindow());
        SDL_DestroyRenderer(getRenderer());
        Mix_CloseAudio();
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }
    SDL_Window*& getWindow(){
        static SDL_Window* window=nullptr;
        return window;
    }
    SDL_Renderer*& getRenderer(){
        static SDL_Renderer* rend=nullptr;
        return rend;
    }
    int getW(){
        return 1280;
    }
    int getH(){
        return 720;
    }
    const char* getTitle(){
        return "Snake";
    }
}

std::pair<bool, const char*> init(){
    int img_flags=IMG_INIT_JPG,sound_flags=MIX_INIT_MP3;
    if (SDL_Init(SDL_INIT_EVERYTHING)){
        return {false, SDL_GetError()};
    }
    if ((IMG_Init(img_flags) & img_flags) != img_flags){
        return {false, IMG_GetError()};
    }
    if (TTF_Init()){
        return {false, TTF_GetError()};
    }
    if ((Mix_Init(sound_flags) & sound_flags) != sound_flags){
        return {false, Mix_GetError()};
    }
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
    window::getWindow() = SDL_CreateWindow(window::getTitle(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window::getW(), window::getH(), 0);
    if (!window::getWindow()){
        return {false, SDL_GetError()};
    }
    window::getRenderer() = SDL_CreateRenderer(window::getWindow(), -1, 0);
    if (!window::getRenderer()){
        return {false, SDL_GetError()};
    }
    return {true, "All Done"};
}
