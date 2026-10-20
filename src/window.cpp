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
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_scancode.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>
#include <SDL_video.h>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <stdexcept>
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
    int sc=0;
    
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
        void reset(){
            sc=0;
            size=3;
            pos.clear();
            int curX=0,curY=0;
            pos.resize(size);
            for (int i=0;i<size;i++){
                auto& a = pos[i];
                a.first=curX;
                a.second=curY;
                curX+=w;
            }
        }
        bool draw(api::image& head, api::image& body, api::image& apple, api::text& t, api::soundeffect se){

            if (pos[size-1].second+32 < 0 || pos[size-1].second+32>window::getH()){return false;}
            if (pos[size-1].first+32 < 0 || pos[size-1].first+32>window::getW()){return false;}
            
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
class slider{
    int minY,maxY;
    SDL_Rect line;
    int curY;
    public:
        slider(int x, int mY, int maY) : minY(mY), maxY(maY)
        {
            line = {x, minY, 6, maxY-minY};
            curY=minY;
        }
        int draw(api::image& lne,api::image& c, SDL_Rect& mouse, SDL_Texture* bluethingy){
            c.getRect().x=line.x-12;
            c.getRect().y=curY;
            lne.getRect()=line;
            lne.draw();
            SDL_Rect dstrect = line;
            dstrect.h=(maxY-curY);
            SDL_RenderCopy(window::getRenderer(), bluethingy, NULL, &dstrect);
            if (mouselistener::isDown(SDL_BUTTON_LEFT)){
                if (SDL_HasIntersection(&line, &mouse)){
                    if (mouse.y >= minY && mouse.y <= maxY){
                        curY=mouse.y;
                    }
                }
            }
            c.draw();
            return curY-minY;
        }
        void setY(int val){
            if (val <= maxY && val >= minY){
                curY=val;
            }
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
        api::image playbutton("assets/images/Play-button.jpg");
        playbutton.getRect().x=380;
        playbutton.getRect().y=220;
        playbutton.getRect().w=128*2;
        playbutton.getRect().h=64*2;
        playbutton.autoset=false;
        api::font f("assets/fonts/Montserrat-BoldItalic.ttf", 24);
        api::text t(f,"Score: 0");
        SDL_Event e;

        SDL_Rect mouse;
        mouse.w=1;
        mouse.h=1;
        std::vector<std::shared_ptr<SDL_Texture>> themes;
        themes.resize(10);
        for (int i=0;i<10;i++){
            std::string st = "assets/themes/theme" + std::to_string(i+1) + ".jpg";
            api::image img(st.c_str());
            themes[i]=img.getTex();
        }

        int th=1;

        int mVol=70,seVol=40;
        api::music m("assets/music/bg.mp3");
        api::soundeffect se("assets/sound-effect/apple.mp3");
        snake s;
        
        api::soundeffect start("assets/sound-effect/play.mp3");
        api::soundeffect over("assets/sound-effect/over.mp3");

        api::image line("assets/images/line.jpg");
        api::image circle("assets/images/round.jpg");

        api::text mu(f, "Music");
        mu.setX(900);
        mu.setY(170);
        slider sl(950, 200, 300);

        api::text su(f, "Sound");
        su.setX(900);
        su.setY(10);
        slider sle(950, 50, 150);

        bool muted=false;
        bool running=true;
        bool play =false;

        api::image mt("assets/images/muted.jpg");
        api::image nmt("assets/images/not muted.jpg");
        
        mt.getRect().x=990;
        mt.getRect().y=getH()-32;
        nmt.getRect()=mt.getRect();

        SDL_Surface* surf = SDL_CreateRGBSurface(0, 32, 32, 16, 0,0,0,0x000000ff);
        if (!surf){
            std::cerr << SDL_GetError() << "\n";
            return;
        }
        SDL_FillRect(surf,NULL, SDL_MapRGB(surf->format, 0, 0, 250));
        SDL_Texture* tex = SDL_CreateTextureFromSurface(getRenderer(), surf);
        SDL_FreeSurface(surf);
        if (!tex){
            std::cerr << SDL_GetError() << "\n";
            return;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distrib = std::uniform_int_distribution<int>(1,10);

        std::vector<api::text> txt;
        txt.emplace_back(f, "press w,s to switch themes 0-9 also does it r to get random");
        txt.emplace_back(f, "m for mute also button in bottom right, sliders for sound");
        txt[1].setY(25);
        txt.emplace_back(f, "arrow keys for movement of snake now click that play button");
        txt[2].setY(50);
        txt.emplace_back(f, "press t again for to show play menu");
        txt[3].setY(75);
        
        bool showtuturial=false;

        std::ifstream file("save.conf");
        if (file.is_open()){
            std::string l;
            int i=0;
            while (std::getline(file, l)){
                if (l.find(' ') == std::string::npos){
                    try {
                        int a = std::stoi(l);
                        if (i==0){mVol=a;}
                        else if(i==1){seVol=a;}
                        else if(i==2){muted=(bool)a;}
                        i++;
                    }
                    catch (std::invalid_argument e){}
                    catch (std::out_of_range e){}
                }
            }
        }
        m.play();
        m.setVolume(mVol);
        se.setVolume(seVol);
        start.setVolume(seVol);
        over.setVolume(seVol);
        if (muted){
            m.setVolume(0);
        }
        sl.setY(mVol+200);
        sle.setY(seVol+50);
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
            if (play){
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
            }
            
            if (keylistener::isPressed(SDLK_m)){
                if (muted){
                    m.setVolume(mVol);
                    se.setVolume(seVol);
                    start.setVolume(seVol);
                    over.setVolume(seVol);
                    muted=false;
                    m.play();
                }
                else {
                    m.setVolume(0);
                    se.setVolume(0);
                    start.setVolume(0);
                    over.setVolume(0);
                    muted=true;
                    m.halt();
                }
            }
            if (keylistener::isPressed(SDLK_t)){
                showtuturial=!showtuturial;
            }
            if (keylistener::isPressed(SDLK_r)){
                th = distrib(gen);
            }
            if (keylistener::isPressed(SDLK_0)){
                th=1;
            }
            else if(keylistener::isPressed(SDLK_1)){
                th=2;
            }
            else if(keylistener::isPressed(SDLK_2)){
                th=3;
            }
            else if(keylistener::isPressed(SDLK_3)){
                th=4;
            }
            else if(keylistener::isPressed(SDLK_4)){
                th=5;
            }
            else if(keylistener::isPressed(SDLK_5)){
                th=6;
            }
            else if(keylistener::isPressed(SDLK_6)){
                th=7;
            }
            else if(keylistener::isPressed(SDLK_7)){
                th=8;
            }
            else if(keylistener::isPressed(SDLK_8)){
                th=9;
            }
            else if(keylistener::isPressed(SDLK_9)){
                th=10;
            }
            
            
            SDL_RenderClear(getRenderer());
            SDL_RenderCopy(getRenderer(), themes[th-1].get(), NULL, NULL);
            if (play){
                t.draw();
                play = s.draw(body, head, apple, t, se);
                if (!play){
                    over.play();
                    s.reset();
                    t.setText("Score: 0");
                    s.dir[0]=1;
                    s.dir[1]=0;
                }
            }
            else {
                if (showtuturial){
                    for (auto& tx : txt){
                        tx.draw();
                    }
                }
                else {
                    mouse.x=mouselistener::getX();
                    mouse.y=mouselistener::getY();
                    if (muted){
                        mt.draw();
                        if (SDL_HasIntersection(&mouse, &mt.getRect()) && mouselistener::isPressed(SDL_BUTTON_LEFT)){
                            m.setVolume(mVol);
                            se.setVolume(seVol);
                            start.setVolume(seVol);
                            over.setVolume(seVol);
                            muted=false;
                            m.play();
                        }
                    }
                    else {
                        nmt.draw();
                        if (SDL_HasIntersection(&mouse, &nmt.getRect()) && mouselistener::isPressed(SDL_BUTTON_LEFT)){
                            m.setVolume(0);
                            se.setVolume(0);
                            start.setVolume(0);
                            over.setVolume(0);
                            muted=true;
                            m.halt();
                        }
                    }
                    playbutton.draw();
                    if (mouselistener::isPressed(SDL_BUTTON_LEFT)){
                        play = SDL_HasIntersection(&mouse, &playbutton.getRect());
                        if (play){
                            start.play();
                        }
                    }
                    mu.draw();
                    mVol=sl.draw(line, circle, mouse, tex);
                    seVol=sle.draw(line, circle, mouse, tex);
                    if (muted){
                    }
                    else {
                        m.setVolume(mVol);
                        se.setVolume(seVol);
                        start.setVolume(seVol);
                        over.setVolume(seVol);
                    }
    
                    su.draw();
                    start.setVolume(seVol);
                    over.setVolume(seVol);
                    se.setVolume(seVol);
                }
            }
            SDL_RenderPresent(getRenderer());

            keylistener::update();
            mouselistener::update();
            SDL_Delay(play ? 192 : 16);
        }
        std::ofstream fi("save.conf", std::ios_base::trunc);
        if (fi.is_open()){
            fi << std::to_string(mVol) << "\n";
            fi << std::to_string(seVol) << "\n";
            fi << std::to_string(muted);
        }
        SDL_DestroyTexture(tex);
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
