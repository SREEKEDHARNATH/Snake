#include "listeners.h"
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_scancode.h>
#include <SDL_stdinc.h>


namespace keylistener {
    bool keys[SDL_NUM_SCANCODES]={false};
    bool prvkeys[SDL_NUM_SCANCODES]={false};
    void reg(SDL_KeyboardEvent key){
        if (key.keysym.scancode > 0 && key.keysym.scancode < SDL_NUM_SCANCODES){
            keys[key.keysym.scancode]=key.state==SDL_PRESSED;
        }
    }

    bool isPressed(SDL_KeyCode k){
        return isPressed(SDL_GetScancodeFromKey(k));
    }
    bool isPressed(SDL_Scancode sc){
        if (sc > 0 && sc < SDL_NUM_SCANCODES){
            return keys[sc] && !prvkeys[sc];
        }
        return false;
    }

    bool isDown(SDL_KeyCode k){
        return isDown(SDL_GetScancodeFromKey(k));
    }
    bool isDown(SDL_Scancode sc){
        if (sc > 0 && sc < SDL_NUM_SCANCODES){
            return keys[sc];
        }
        return false;
    }

    void update(){
        for (int i=0; i<SDL_NUM_SCANCODES;i++){
            prvkeys[i]=keys[i];
        }
    }
}

namespace mouselistener {
    bool buttons[3]={false};
    bool prvbuttons[3]={false};
    int x=0,y=0;
    float scrlx=0,scrly=0;
    void reg(SDL_MouseMotionEvent m){
        x=m.x;
        y=m.y;
    }
    void reg(SDL_MouseButtonEvent b){
        if (b.button > 0 && b.button < 4){
            buttons[b.button-1] = (b.state==SDL_PRESSED);
        }
    }
    void reg(SDL_MouseWheelEvent w){
        scrlx=w.x;
        scrly=w.y;
    }

    bool isPressed(Uint8 b){
        if (b > 0 && b < 4){
            return buttons[b-1] && !prvbuttons[b-1];
        }
        return false;
    }
    bool isDown(Uint8 b){
        if (b > 0 && b < 4){
            return buttons[b-1];
        }
        return false;
    }

    int getX(){
        return x;
    }
    int getY(){
        return y;
    }

    float getScrollX(){
        return scrlx;
    }
    float getScrollY(){
        return scrly;
    }
    void update(){
        for (int i=0;i<3;i++){
            prvbuttons[i]=buttons[i];
        }
    }
}
