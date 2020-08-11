/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/

#include "../states.h"
#include "../objectlist.h"
#include "Object.h"
#include <SDL2/SDL.h>
char Player_frame(ac_object *self,ac_state** current,ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags)
{
    return 0;
}
void Player_destroy(ac_object **self)
{
    free((*self)->objdata);
    free(*self);
    *self=NULL;
}
ac_object *Player(double x, double y, char team)
{
    ac_object *ret=malloc(sizeof(ac_object));
    ret->objdata=malloc(sizeof(player_data));
    ret->destroy=Player_destroy;
    ret->frame=Player_frame;
    return ret;
}