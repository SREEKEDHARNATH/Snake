#include "window.h"
#include "utils.h"
#include "listeners.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <fstream>
#include <iostream>

std::pair<bool, const char*> init();

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

        std::vector<std::shared_ptr<SDL_Texture>> colors(10);
        
        for (int i=0;i<7;i++){
            Uint8 r=0,g=0,b=0;
            if (i==0){
                r=255;
                g=200;
            }
            else if(i==1){
                g=255;
            }
            else if(i==2){
                b=255;
                r=200;
            }
            else if(i==3){
                r=255;
                g=255;
            }
            else if(i==4){
                r=255;
                b=255;
            }
            else if(i==5){
                g=255;
                b=255;
            }
            else {
                r=20;
                g=200;
                b=200;
            }
            
            SDL_Texture* tex = getRgb(r,g,b);
            if (!tex){
                return;
            }
            colors[i].reset(tex, SDL_DestroyTexture);
        }
        SDL_Texture* tex = getRgb(0, 0, 255);
        if (!tex){
            return;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distrib = std::uniform_int_distribution<int>(1,10);

        std::vector<api::text> txt;
        txt.emplace_back(f, "press w,s to switch themes 0-9 also does it r to get random");
        txt.emplace_back(f, "m for mute also button in bottom right, invert sliders for sound");
        txt[1].setY(25);
        txt.emplace_back(f, "arrow keys for movement of snake, now click that play button");
        txt[2].setY(50);
        txt.emplace_back(f, "press t again to show play menu");
        txt[3].setY(75);
        txt.emplace_back(f, "Hope you like the game");
        txt[4].setY(100);
        
        api::text presst(f, "Press t", {252, 227, 30, 255});
        presst.setY(500);
        bool showtuturial=false;

        bool showSolidColor=false;
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
                        else if(i==3){showSolidColor=(bool)a;}
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
        checkbox ch(780, 520);
        ch.setChecked(showSolidColor);
        api::text showsolidcolortext(f, "Show Solid Color");
        showsolidcolortext.setY(490);
        showsolidcolortext.setX(700);
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
            mouse.x=mouselistener::getX();
            mouse.y=mouselistener::getY();
            
            if (keylistener::isDown(SDLK_ESCAPE)){
                running=false;
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
            
            if (keylistener::isPressed(SDLK_t) && !play){
                showtuturial=!showtuturial;
            }

            if (keylistener::isPressed(SDLK_s)){
                if (th>1){
                    th--;
                }
            }
            if (keylistener::isPressed(SDLK_w)){
                if (th<(showSolidColor ? 7 : 10)){
                    th++;
                }
            }
            if (keylistener::isPressed(SDLK_r)){
                th = std::min(7,distrib(gen));
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
                if (!showSolidColor){
                    th=8;
                }
            }
            else if(keylistener::isPressed(SDLK_8)){
                if (!showSolidColor){
                    th=9;
                }
            }
            else if(keylistener::isPressed(SDLK_9)){
                if (!showSolidColor){
                    th=10;
                }
            }
            
            
            SDL_RenderClear(getRenderer());
            SDL_RenderCopy(getRenderer(), showSolidColor ? colors[std::min(6,th-1)].get() : themes[th-1].get(), NULL, NULL);
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

                    showsolidcolortext.draw();
                    showSolidColor=ch.draw(mouse);
                    presst.draw();
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
            fi << std::to_string(muted) << "\n";
            fi << std::to_string(showSolidColor); 
        }
        SDL_DestroyTexture(tex);
    }
    void quit(){
        SDL_DestroyRenderer(getRenderer());
        SDL_DestroyWindow(getWindow());
        Mix_CloseAudio();
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }
    SDL_Window*& getWindow(){
        static SDL_Window* window_ptr=nullptr;
        return window_ptr;
    }
    SDL_Renderer*& getRenderer(){
        static SDL_Renderer* renderer_ptr=nullptr;
        return renderer_ptr;
    }
    int getW(){
        return 1024;
    }
    int getH(){
        return 576;
    }
    const char* getTitle(){
        return "Snake Game";
    }
}

std::pair<bool, const char*> init(){
    int img_flags=IMG_INIT_JPG;
    int sound_flags=MIX_INIT_MP3;
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
