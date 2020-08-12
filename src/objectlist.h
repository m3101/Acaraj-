/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/

#ifndef AC_OBJIST
#define AC_OBJLIST
#include "states.h"
#include "Objects/Object.h"
#include "Logic/ll.h"
#include "Maths/Vect2d/Vect2d.h"
/*
Player
*/
/*x,y*/
double playerDefault[20];
typedef struct player_data{
    double target[20];
    double cur[20];
    double prev[20];
    double forceAcc[20];
    unsigned char team;
    unsigned char mode;
}player_data;
char Player_frame(ac_object *self,ac_state** current,ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags);
void Player_destroy(ac_object** self);
ac_object *Player(double x, double y, unsigned char team);
#endif