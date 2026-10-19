#pragma once

#include <SDL_events.h>
#include <SDL_keycode.h>
namespace keylistener {
    void reg(SDL_KeyboardEvent key);

    bool isPressed(SDL_KeyCode k);
    bool isPressed(SDL_Scancode sc);

    bool isDown(SDL_KeyCode k);
    bool isDown(SDL_Scancode sc);
    void update();
}
namespace mouselistener {
    void reg(SDL_MouseMotionEvent m);
    void reg(SDL_MouseButtonEvent b);
    void reg(SDL_MouseWheelEvent w);

    bool isPressed(Uint8 b);
    bool isDown(Uint8 b);

    int getX();
    int getY();

    float getScrollX();
    float getScrollY();
    void update();
}
