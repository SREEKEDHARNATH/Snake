#include "api.h"
#include "window.h"
#include <SDL2/SDL_image.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>
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

    font::font(const char* name, int ptsize){
        TTF_Font* f=TTF_OpenFont(name, ptsize);
        if (!f){
            std::cerr << TTF_GetError() << "\n";
            return;
        }
        fnt.reset(f, TTF_CloseFont);
    }
    std::shared_ptr<TTF_Font> font::getFont(){
        return fnt;
    }

    text::text(font& f, const char* text, SDL_Color fg){
        if (f.getFont().get()){
            fnt = f.getFont();
            setText(text, fg);
        }
    }
    void text::setText(const char* text, SDL_Color fg){
        if (fnt.get()){
            SDL_Surface* surf = TTF_RenderUTF8_Blended(fnt.get(), text, fg);
            if (!surf){
                std::cerr << TTF_GetError() << "\n";
                return;
            }
            SDL_Texture* tex = SDL_CreateTextureFromSurface(window::getRenderer(), surf);
            SDL_FreeSurface(surf);
            if (!tex){
                std::cerr << SDL_GetError() << "\n";
                return;
            }
            img.reset(tex, SDL_DestroyTexture);
            int tW,tH;
            SDL_QueryTexture(img.get(), NULL, NULL, &tW, &tH);
            dstrect.w=tW;
            dstrect.h=tH;
        }
    }
    void text::draw(){
        if (img.get()){
            SDL_RenderCopy(window::getRenderer(), img.get(), NULL, &dstrect);
        }
    }
    void text::setX(int val){
        dstrect.x=val;
    }
    void text::setY(int val){
        dstrect.y=val;
    }
    int text::getX(){
        return dstrect.x;
    }
    int text::getY(){
        return dstrect.y;
    }


}
