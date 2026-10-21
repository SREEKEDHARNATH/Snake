#include "utils.h"
#include "listeners.h"
#include "window.h"
#include <iostream>
#include <map>

snake::snake(){
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


void snake::reset(){
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


bool snake::draw(api::image& head, api::image& body, api::image& apple, api::text& t, api::soundeffect se){

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


slider::slider(int x, int mY, int maY) : minY(mY), maxY(maY)
{
    line = {x, minY, 6, maxY-minY};
    curY=minY;
}

int slider::draw(api::image& lne,api::image& c, SDL_Rect& mouse, SDL_Texture* bluethingy){
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

void slider::setY(int val){
    if (val <= maxY && val >= minY){
        curY=val;
    }
}


SDL_Texture* getRgb(Uint8 r,Uint8 g,Uint8 b){
    SDL_Surface* surf = SDL_CreateRGBSurface(0, 32, 32, 16, 0,0,0,0x000000ff);
    if (!surf){
        std::cerr << SDL_GetError() << "\n";
        return NULL;
    }
    SDL_FillRect(surf,NULL, SDL_MapRGB(surf->format, r,g,b));
    SDL_Texture* tex = SDL_CreateTextureFromSurface(window::getRenderer(), surf);
    SDL_FreeSurface(surf);
    if (!tex){
        std::cerr << SDL_GetError() << "\n";
        return NULL;
    }
    return tex;
}


checkbox::checkbox(int x,int y){
    check=false;
    api::image ch("assets/images/filled-box.jpg");
    api::image uch("assets/images/unfilled-box.jpg");
            
    checked=ch.getTex();
    unchecked=uch.getTex();
            
    dstrect.x=x;
    dstrect.y=y;
    dstrect.w=24;
    dstrect.h=24;
}

bool checkbox::draw(SDL_Rect& mouse){
    if (check){
        SDL_RenderCopy(window::getRenderer(), checked.get(), NULL, &dstrect);
    }
    else {
        SDL_RenderCopy(window::getRenderer(), unchecked.get(), NULL, &dstrect);
    }
    if (SDL_HasIntersection(&mouse, &dstrect) && mouselistener::isPressed(SDL_BUTTON_LEFT)){
        check=!check;
    }
    return check;
}

void checkbox::setChecked(bool ch){
    check=ch;
}