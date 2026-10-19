#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>
#include <memory>
#include <SDL2/SDL_ttf.h>

namespace api {
    class image{
        std::shared_ptr<SDL_Texture> img;
        SDL_Rect dstrect={0};
        public:
            bool autoset=true;
            image(const char* name);
            void draw();
            SDL_Rect& getRect();
            std::shared_ptr<SDL_Texture> getTex();
    };
    class font{
        std::shared_ptr<TTF_Font> fnt;
        public:
            font(const char* name, int ptsize);
            std::shared_ptr<TTF_Font> getFont();
    };
    class text{
        std::shared_ptr<TTF_Font> fnt;
        std::shared_ptr<SDL_Texture> img;
        SDL_Rect dstrect={0};
        public:
            text(font& f, const char* text, SDL_Color fg={255,255,255,255});
            void setText(const char* text, SDL_Color fg={255,255,255,255});
            void draw();
            void setX(int val);
            void setY(int val);
            int getX();
            int getY();

    };
}
