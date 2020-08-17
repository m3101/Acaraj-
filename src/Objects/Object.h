/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/

#ifndef AC_OBJECT
#include <SDL2/SDL.h>
#include "../states.h"
#include "../Maths/Vect2d/Vect2d.h"
#define AC_OBJECT
/*
Note: if "frame" returns -1, the object is destroyed.
*/
typedef struct ac_object{
    void* objdata;
    char (*frame)(struct ac_object *self,ac_state** current,ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags);
    void (*destroy)(struct ac_object **self);
    void (*event)(struct ac_object *self,ac_state** current,ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags,SDL_Event* evt);
    char (*collideEdge)(vect2d pos,vect2d dir,ray2d* projection,struct ac_object *self,ac_state** current,ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags,SDL_Event* evt,char col_flags);
}ac_object;
#endif