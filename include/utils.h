#pragma once

#include "api.h"
#include <SDL_rect.h>
#include <random>
#include <utility>
#include <vector>
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
        snake();
        void reset();
        bool draw(api::image& head, api::image& body, api::image& apple, api::text& t, api::soundeffect se);
};

class slider{
    int minY,maxY;
    SDL_Rect line;
    int curY;
    public:
        slider(int x, int mY, int maY);
        int draw(api::image& lne,api::image& c, SDL_Rect& mouse, SDL_Texture* bluethingy);
        void setY(int val);
};
SDL_Texture* getRgb(Uint8 r,Uint8 g,Uint8 b);
class checkbox{
    std::shared_ptr<SDL_Texture> checked;
    std::shared_ptr<SDL_Texture> unchecked;
    SDL_Rect dstrect;

    bool check;
    public:
        checkbox(int x,int y);
        bool draw(SDL_Rect& mouse);
        void setChecked(bool ch);
};