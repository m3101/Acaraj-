/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/

#include "../states.h"
#include "../objectlist.h"
#include "../Logic/ll.h"
#include "../Maths/Vect2d/Vect2d.h"
#include "../Rendering/sqline.h"
#include "Object.h"
#include <SDL2/SDL.h>

double playerDefault[]={
/*00,01*/    0,0,
/*02,03*/    0,10,
/*04,05*/    5,15,
/*06,07*/    10,10,
/*08,09*/    10,0,
/*10,11*/    5,35,
/*12,13*/    0,20,
/*14,15*/    5,40,
/*16,17*/    10,20,
/*18,19*/    20,32
};

/*
Rigidbody system based on
"Advanced Character Physics"
Jakobsen, Thomas
http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.206.4908&rep=rep1&type=pdf

It's basically a https://en.wikipedia.org/wiki/Jacobi_method solution of a series of constraints.
Our equations are the following constraints:
C0: No vertex should be inside other objects
C1: The foot nodes should be as close as possible to their targets
C2: The distances between the nodes should equal their target's distances
*/

char Player_frame(ac_object *self,ac_state** current,ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags)
{
    unsigned char i,i2,squigliness=2;
    double squigglelen=4;
    double *cur=((player_data*)(self->objdata))->cur;
    /*Draw the player*/
    unsigned char r=((player_data*)(self->objdata))->team==0?255:0,
                  g=((player_data*)(self->objdata))->team==1?255:0,
                  b=((player_data*)(self->objdata))->team==2?255:0;
    SDL_SetRenderDrawColor(renderer, r,g,b, SDL_ALPHA_OPAQUE);
    /*Legs*/
    squigglyLine(cur[0],cur[1],cur[2],cur[3],squigglelen,squigliness,renderer);
    squigglyLine(cur[2],cur[3],cur[4],cur[5],squigglelen,squigliness,renderer);

    squigglyLine(cur[4],cur[5],cur[6],cur[7],squigglelen,squigliness,renderer);
    squigglyLine(cur[6],cur[7],cur[8],cur[9],squigglelen,squigliness,renderer);
    /*Body*/
    squigglyLine(cur[4],cur[5],cur[10],cur[11],squigglelen,squigliness,renderer);
    /*Arms and sword*/
    squigglyLine(cur[10],cur[11],cur[12],cur[13],squigglelen,squigliness,renderer);
    squigglyLine(cur[10],cur[11],cur[16],cur[17],squigglelen,squigliness,renderer);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, SDL_ALPHA_OPAQUE);
    squigglyLine(cur[16],cur[17],cur[18],cur[19],squigglelen,squigliness,renderer);
    SDL_SetRenderDrawColor(renderer, r,g,b, SDL_ALPHA_OPAQUE);
    /*Head*/
    squigglyLine(cur[10],cur[11],cur[14],cur[15],squigglelen,squigliness+2,renderer);
    return 0;
}
void Player_destroy(ac_object **self)
{
    free((*self)->objdata);
    free(*self);
    *self=NULL;
}

/*
Player vertex structure.
The numbers are the indices of the x coordinate on the vertex lists
            18
    14      /
     |     /
12__10___16
     |
     |
    _4_
   /   \
   2   6
   |   |
  *0   8
*/
void initVertices(ac_object* player,double x, double y,double* pattern)
{
    for(char i=0;i<20;i++)
    {
        ((player_data*)(player->objdata))->cur[i]=(i%2==0?pattern[i]+x:480-(pattern[i]+y));
    }
}

ac_object *Player(double x, double y, unsigned char team)
{
    ac_object *ret=malloc(sizeof(ac_object));
    ret->objdata=malloc(sizeof(player_data));
    ret->destroy=Player_destroy;
    ret->frame=Player_frame;
    initVertices(ret,x,y,playerDefault);
    ((player_data*)ret->objdata)->team=team;
    ((player_data*)ret->objdata)->mode=0;
    return ret;
}