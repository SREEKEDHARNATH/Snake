#include "window.h"
#include "api.h"
#include "listeners.h"
#include <SDL.h>
#include <SDL_audio.h>
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
#include <map>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>
std::pair<bool, const char*> init();

class snake{
    std::vector<std::pair<int, int>> pos;
    const int w=32;
    const int h=32;
    const int vel=32;
    int size=3;
    SDL_Rect appledstrect={320,320,32,32};
    std::mt19937 gen;
    std::uniform_int_distribution<int> distribX;
    std::uniform_int_distribution<int> distribY;
    
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
            std::random_device rd;
            gen = std::mt19937(rd());

            int lastX;
            for (lastX=0;lastX<window::getW();lastX+=32);
            lastX-=32;
            distribX = std::uniform_int_distribution<int>(0,lastX/32);

            int lastY;
            for (lastY=0;lastY<window::getH();lastY+=32);
            lastY-=32;
            distribY = std::uniform_int_distribution<int>(0,lastY/32);
            
        }
        bool draw(api::image& head, api::image& body, api::image& apple, api::text& t, api::soundeffect se){
            static int sc=0;

            if (pos[size-1].second < 0 || pos[size-1].second>window::getH()){return false;}
            if (pos[size-1].first < 0 || pos[size-1].first>window::getW()){return false;}
            
            if (appledstrect.x==-100){
                int ax= distribX(gen)*32;
                int ay= distribY(gen)*32;
                std::map<int,bool> allx;
                std::map<int,bool> ally;
                for (int i=0;i<size;i++){
                    allx[pos[i].first]=true;
                    ally[pos[i].second]=true;
                }
                for (int i=0;i<allx.size();i++){
                    if (allx[ax]){
                        int ax= distribX(gen)*32;
                    }
                    if (ally[ay]){
                        int ay= distribY(gen)*32;
                    }
                }
                appledstrect.x=ax;
                appledstrect.y=ay;
            }

            apple.getRect()=appledstrect;
            apple.draw();
            
            auto& he = head.getRect();
            auto& bo = body.getRect();
            
            bo.w=w;
            bo.h=h;

            he.w=w;
            he.h=h;
            auto old=pos[0];
            
            he.x=pos[size-1].first;
            he.y=pos[size-1].second;
            head.draw();
            for (int i=0;i<size-1;i++){
                bo.x=pos[i].first;
                bo.y=pos[i].second;
                body.draw();
                pos[i]=pos[i+1];
            }

            if (SDL_HasIntersection(&he, &appledstrect)){
                se.play();
                appledstrect.x=-100;
                std::vector<std::pair<int, int>> newpos;
                newpos.resize(size+1);
                for (int i=0;i<size;i++){
                    newpos[i+1]=pos[i];
                }
                newpos[0]=old;
                pos.clear();
                pos=newpos;
                size++;
                sc++;
                std::string str="Score: " + std::to_string(sc);
                t.setText(str.c_str());
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
        api::image body("assets/images/body.jpg");
        api::image head("assets/images/head.jpg");
        api::image apple("assets/images/apple.jpg");
        api::font f("assets/fonts/Montserrat-BoldItalic.ttf", 32);
        api::text t(f,"Score: 0");
        SDL_Event e;

        std::vector<std::shared_ptr<SDL_Texture>> themes;
        themes.resize(10);
        for (int i=0;i<10;i++){
            std::string st = "assets/themes/theme" + std::to_string(i+1) + ".jpg";
            api::image img(st.c_str());
            themes[i]=img.getTex();
        }

        int th=1;

        constexpr int mVol=30,seVol=20;
        api::music m("assets/music/bg.mp3");
        m.setVolume(mVol);
        api::soundeffect se("assets/sound-effect/apple.mp3");
        se.setVolume(seVol);
        m.play();
        snake s;

        bool muted=false;
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

            if (keylistener::isPressed(SDLK_s)){
                if (th>1){
                    th--;
                }
            }
            if (keylistener::isPressed(SDLK_w)){
                if (th<10){
                    th++;
                }
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

            if (keylistener::isPressed(SDLK_m)){
                if (muted){
                    m.setVolume(mVol);
                    se.setVolume(seVol);
                    muted=false;
                }
                else {
                    m.setVolume(0);
                    se.setVolume(0);
                    muted=true;
                }
            }

            SDL_RenderClear(getRenderer());
            SDL_RenderCopy(getRenderer(), themes[th-1].get(), NULL, NULL);
            if (running){
                t.draw();
                running = s.draw(body, head, apple, t, se);
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
        return 1024;
    }
    int getH(){
        return 576;
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
