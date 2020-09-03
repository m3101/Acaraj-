/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/

#include "../states.h"
#include "../objectlist.h"
#include "../statelist.h"
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
/*18,19*/    25,37
};
unsigned char PlayerLinks[]={
    0,2,
    2,4,
    4,6,
    6,8,
    4,10,
    10,12,
    10,14,
    10,16,
    16,18
};

void initVerticesT(ac_object* player,int x, int y,double* target,double* pattern)
{
    for(char i=0;i<20;i++)
    {
        target[i]=pattern[i];
    }
}
void initVertices(ac_object* player,int x, int y,double* pattern)
{
    for(char i=0;i<20;i++)
    {
        ((player_data*)(player->objdata))->cur[i]=(i%2==0?pattern[i]+x:480-(pattern[i]+y));
        ((player_data*)(player->objdata))->prev[i]=((player_data*)(player->objdata))->cur[i];
    }
}

/*
Rigidbody system based on
"Advanced Character Physics"
Jakobsen, Thomas
http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.206.4908&rep=rep1&type=pdf

It's basically a https://en.wikipedia.org/wiki/Jacobi_method solution of a series of constraints.
Our equations are the following constraints, evaluated in this order:
C1: The other nodes should seek their target positions (non-rigid)
C0: No vertex should be inside other objects
C3: The distances between the nodes should equal their target's distances

Player stateFlags:
01 = "right" key pressed
02 = "left" key pressed
04 = "up" key pressed (target1)
010= "down" key pressed (target2)
011= "target3" key pressed
012= "target4" key pressed
013= "target5" key pressed
014= Facing left (mirror everything)
*/
char Player_frame(ac_object *self,ac_state** current,ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags)
{
    unsigned char i,i2,j,skipvx,squigliness=2,target,ixa,iya,ixb,iyb,
                  *animstate=&((player_data*)(self->objdata))->animstate,
                  *stateflags=&((player_data*)(self->objdata))->stateFlags;
    double squigglelen=4;
    double *cur=((player_data*)(self->objdata))->cur,
           *prev=((player_data*)(self->objdata))->prev,
           *target0=((player_data*)(self->objdata))->target0,
           *target1=((player_data*)(self->objdata))->target1,
           *target2=((player_data*)(self->objdata))->target2,
           *target3=((player_data*)(self->objdata))->target3,
           *target4=((player_data*)(self->objdata))->target4,
           *targets[]={target0,target1,target2,target3,target4},
           tdist,dist,temp;
    int *x=&((player_data*)(self->objdata))->x,*y=&((player_data*)(self->objdata))->y,dx,dy;
    skipvx=((player_data*)(self->objdata))->animstate;
    /*The current target*/
    if(*stateflags&020)target=3;
    else if(*stateflags&040)target=4;
    else if(*stateflags&04)target=1;
    else if(*stateflags&010)target=2;
    else target=0;

    const double wobbliness=.8;
    /*Inertia/Speed effect*/
    for(i=0;i<10;i++)
    {
        i2=2*i;
        if((*stateflags&01||*stateflags&02)&&i2==skipvx)
            continue;
        temp=cur[i2];
        cur[i2]+=wobbliness*(cur[i2]-prev[i2]);
        prev[i2]=temp;
        temp=cur[i2+1];
        cur[i2+1]+=wobbliness*(cur[i2+1]-prev[i2+1]);
        prev[i2+1]=temp;
    }

    /*Calculate distances and constraints*/
    cdc:
    /*Non-rigid constraints*/
    /*C1*/
    for(i=0;i<10;i++)
    {
        i2=2*i;
        if((*stateflags&01||*stateflags&02)&&i2==skipvx)
            continue;
        dx=(targets[target][i2]+*x)-cur[i2];
        dy=(targets[target][i2+1]+(480-*y))-cur[i2+1];
        dist=dx;
        cur[i2]+=dx*.4;
        dist=dy;
        cur[i2+1]+=dy*.4;
    }
    /*Rigid constraints*/
    for(j=0;j<2;j++)
    {
        for(i=0;i<9;i++)
        {
            i2=2*i;
            ixa=PlayerLinks[i2];
            iya=ixa+1;
            ixb=PlayerLinks[i2+1];
            iyb=ixb+1;
            dx=targets[target][ixa]-targets[target][ixb];
            dy=targets[target][iya]-targets[target][iyb];
            /*Target Distance*/
            tdist=sqrt(dx*dx+dy*dy);
            /*C0 Colllisions*/
            /*C3 Distances*/
            dx=cur[ixb]-cur[ixa];
            dy=cur[iyb]-cur[iya];
            dist=(sqrt(dx*dx+dy*dy)-tdist)/tdist;
            if(isnan(dist)||isinf(dist))
            {
                /*Panic*/
                initVertices(self,*x,*y,playerDefault);
                initVerticesT(self,0,0,targets[target],playerDefault);
                goto cdc;
            }
            if(ixa!=skipvx||(!(*stateflags&01||*stateflags&02)))
            {
                cur[ixa]+=dx*dist*.3;
                cur[iya]+=dy*dist*.3;
            }
            if(ixb!=skipvx||(!(*stateflags&01||*stateflags&02)))
            {
                cur[ixb]-=dx*dist*.3;
                cur[iyb]-=dy*dist*.3;
            }
        }
    }

    /*Walk animation*/
    if(*stateflags&01||*stateflags&02)
    {
        dx=cur[0]-cur[8];
        dist=sqrt(dx*dx);
        dx=targets[target][0]-targets[target][8];
        tdist=sqrt(dx*dx);
        if(dist>tdist||dist<3)
            *animstate=*animstate==0?8:0;
        /*Walk position update*/
        if(*stateflags&01)*x+=3;
        else *x-=3;
    }

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
    squigglyLine(cur[10],cur[11],cur[14],cur[15],squigglelen,squigliness,renderer);
    return 0;
}
void Player_event(ac_object *self,ac_state** current,ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags,SDL_Event* evt)
{
    if(evt->type==SDL_KEYDOWN)
    {
        for(unsigned char i=0;i<6;i++)
        if(evt->key.keysym.sym==((player_data*)self->objdata)->controls[i])
        {
            ((player_data*)(self->objdata))->stateFlags|=(unsigned char)1<<i;
        }
    }
    else if(evt->type==SDL_KEYUP)
    {
        for(unsigned char i=0;i<6;i++)
        if(evt->key.keysym.sym==((player_data*)self->objdata)->controls[i])
        {
            ((player_data*)(self->objdata))->stateFlags&=~((unsigned char)1<<i);
        }
    }
}
void Player_destroy(ac_object **self)
{
    free((*self)->objdata);
    free(*self);
    *self=NULL;
}

char Player_collideEdge(vect2d *pos,vect2d *dir,ray2d* projection,struct ac_object *self,ac_state** current,ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags,SDL_Event* evt,char col_flags)
{
    return 0;
    unsigned char i2;
    unsigned char n=0,killflag=0;
    ray2d ret;
    vect2d p,pdir,ea,eb;
    double s1,s2,sc;
    ret.dirscale.i=0;
    ret.dirscale.j=0;
    ret.pos.i=0;
    ret.pos.j=0;
    /*For each edge, check whether the vector is intersecting*/
    for(unsigned char i=0;i<9;i++)
    {
        i2=2*i;
        ea.i=((player_data*)self->objdata)->cur[PlayerLinks[i2]];
        ea.j=((player_data*)self->objdata)->cur[PlayerLinks[i2]+1];
        eb.i=((player_data*)self->objdata)->cur[PlayerLinks[i2+1]];
        eb.j=((player_data*)self->objdata)->cur[PlayerLinks[i2+1]+1];
        sub2d(&eb,&ea,&pdir);
        sc=crossProductMagnitude(dir,&pdir);
        sub2d(pos,&ea,&p);
        s1=crossProductMagnitude(&p,&pdir)/sc;
        if(s1>1||s1<=0)continue;
        sub2d(&ea,pos,&p);
        s2=crossProductMagnitude(&p,dir)/sc;
        if(s2>1||s2<=0)continue;
        p.i=-dir->i;
        p.j=-dir->j;
        scale2d(&p,s2,&p);
        ret.pos.i+=9;
    }
    projection->dirscale.i=ret.dirscale.i;
    projection->dirscale.j=ret.dirscale.j;
    projection->pos.i=ret.pos.i;
    projection->pos.j=ret.pos.j;
    return killflag?-n:n;
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

ac_object *Player(double x, double y, unsigned char team,ll* world)
{
    ac_object *ret=malloc(sizeof(ac_object));
    ret->objdata=malloc(sizeof(player_data));
    ret->destroy=Player_destroy;
    ret->frame=Player_frame;
    ret->event=Player_event;
    ret->collideEdge=Player_collideEdge;
    if(team==0)
    {
        ((player_data*)ret->objdata)->controls[0]=SDLK_d;
        ((player_data*)ret->objdata)->controls[1]=SDLK_a;
        ((player_data*)ret->objdata)->controls[2]=SDLK_w;
        ((player_data*)ret->objdata)->controls[3]=SDLK_s;
        ((player_data*)ret->objdata)->controls[4]=SDLK_c;
        ((player_data*)ret->objdata)->controls[5]=SDLK_v;
    }
    initVertices(ret,0,0,playerDefault);
    initVerticesT(ret,0,0,((player_data*)ret->objdata)->target0,designer_states[0]);
    initVerticesT(ret,0,0,((player_data*)ret->objdata)->target1,designer_states[1]);
    initVerticesT(ret,0,0,((player_data*)ret->objdata)->target2,designer_states[2]);
    initVerticesT(ret,0,0,((player_data*)ret->objdata)->target3,designer_states[3]);
    initVerticesT(ret,0,0,((player_data*)ret->objdata)->target4,designer_states[4]);
    ((player_data*)ret->objdata)->team=team;
    ((player_data*)ret->objdata)->stateFlags=0;
    ((player_data*)ret->objdata)->animstate=0;
    ((player_data*)ret->objdata)->x=x;
    ((player_data*)ret->objdata)->y=y;
    ((player_data*)ret->objdata)->world=world;
    return ret;
}