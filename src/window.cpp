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
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <SDL_video.h>
#include <iostream>
#include <utility>
#include <vector>
std::pair<bool, const char*> init();

class snake{
    std::vector<std::pair<int, int>> pos;
    int w=32,h=32;
    int vel=32;
    int size=10;
    public:
        int dir[2]={1,0};
        snake(){
            int curX=0,curY=0;
            pos.resize(size);
            for (int i=0;i<size;i++){
                auto& a = pos[i];
                a.first=curX;
                a.second=curY;
                curX+=w;
            }
        }
        bool draw(api::image& head, api::image& body){
            if (pos[size-1].second < 0 || pos[size-1].second>window::getH()){return false;}
            if (pos[size-1].first < 0 || pos[size-1].first>window::getW()){return false;}

            auto& he = head.getRect();
            auto& bo = body.getRect();
            
            bo.w=w;
            bo.h=h;

            he.w=w;
            he.h=h;
            
            he.x=pos[size-1].first;
            he.y=pos[size-1].second;
            
            head.draw();
            for (int i=0;i<size-1;i++){
                bo.x=pos[i].first;
                bo.y=pos[i].second;
                body.draw();
            }
            for (int i=0;i<size-1;i++){
                pos[i]=pos[i+1];
            }
            pos[size-1].first+=vel*dir[0];
            pos[size-1].second+=vel*dir[1];
            
            for (int i=0;i<size-1;i++){
                if (pos[i]==pos[size-1]){
                    return false;
                }
            }
            return true;
        }
};
namespace window {
    void run(){
        auto i = init();
        if (!i.first){
            std::cerr << i.second << "\n";
            return;
        }
        api::image img("assets/images/blue-square.jpg");
        api::image img2("assets/images/blue-square.jpg");
        api::font f("assets/fonts/Montserrat-BoldItalic.ttf", 32);
        api::text t(f, "Hello");
        SDL_Event e;

        snake s;
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

            if (keylistener::isDown(SDLK_ESCAPE)){
                running=false;
            }
            if (keylistener::isDown(SDLK_UP)){
                if (s.dir[0]){
                    s.dir[1]=-1;
                    s.dir[0]=0;
                }
            }
            else if (keylistener::isDown(SDLK_DOWN)){
                if (s.dir[0]){
                    s.dir[1]=1;
                    s.dir[0]=0;
                }
            }
            else if (keylistener::isDown(SDLK_RIGHT)){
                if (s.dir[1]){
                    s.dir[0]=1;
                    s.dir[1]=0;
                }
            }
            else if (keylistener::isDown(SDLK_LEFT)){
                if (s.dir[1]){
                    s.dir[0]=-1;
                    s.dir[1]=0;
                }
            }
            SDL_RenderClear(getRenderer());
            t.draw();
            if (running){
                running = s.draw(img, img2);
            }
            SDL_RenderPresent(getRenderer());

            keylistener::update();
            mouselistener::update();
            SDL_Delay(192);
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
