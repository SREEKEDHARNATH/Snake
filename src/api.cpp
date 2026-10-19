#include "api.h"
#include "window.h"
#include <SDL2/SDL_image.h>
#include <SDL_render.h>
#include <iostream>

namespace api {
    image::image(const char* name){
        SDL_Texture* tex = IMG_LoadTexture(window::getRenderer(), name);
        if (!tex){
            std::cerr << IMG_GetError() << "\n";
            return;
        }
        img.reset(tex, SDL_DestroyTexture);
    }
    void image::draw(){
        if (img.get()){
            if (autoset){
                int tW,tH;
                SDL_QueryTexture(img.get(), NULL, NULL, &tW, &tH);
                dstrect.w=tW;
                dstrect.h=tH;
            }
            if (dstrect.w <= 0 || dstrect.h <= 0){
                SDL_RenderCopy(window::getRenderer(), img.get(), NULL, NULL);
            }
            else {
                SDL_RenderCopy(window::getRenderer(), img.get(), NULL, &dstrect);
            }
            autoset=false;
        }
    }
    SDL_Rect& image::getRect(){
        return dstrect;
    }
    std::shared_ptr<SDL_Texture> image::getTex(){
        return img;
    }
}
