#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>
#include <memory>
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
}
