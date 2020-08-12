/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/

#include "../states.h"
#include "../statelist.h"
#include "../Rendering/sqline.h"
#include <SDL2/SDL.h>

double dists[]={
    0,2,0,
    2,4,0,
    4,6,0,
    6,8,0,
    4,10,0,
    10,12,0,
    10,14,0,
    10,16,0,
    16,18,0
};
double defaultEdit[]={
/*00,01*/    0,0,
/*02,03*/    0,10,
/*04,05*/    5,15,
/*06,07*/    10,10,
/*08,09*/    10,0,
/*10,11*/    5,35,
/*12,13*/    0,20,
/*14,15*/    5,40,
/*16,17*/    10,20,
///*18,19*/    20,32
};
double playerEdit[]={
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
const double playerDefaultEdit[]={
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

unsigned char brokenflag=0;
void reset_edit()
{
    for(char i=0;i<20;i++)
        playerEdit[i]=defaultEdit[i];
}

struct {} DesignerState_Vars={};
void DesignerState_init(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags)
{
    printf("In designer state.\n");
    unsigned char i,i3,ixa,iya,ixb,iyb;
    for(i=0;i<20;i++)
    {
        playerEdit[i]=playerDefaultEdit[i];
        playerEdit[i]*=10;
        if(i%2==1)
            playerEdit[i]=460-playerEdit[i];
        else playerEdit[i]+=300;
        defaultEdit[i]=playerEdit[i];
    }
    for(i=0;i<9;i++)
    {
        i3=3*i;
        ixa=dists[i3];
        iya=dists[i3]+1;
        ixb=dists[i3+1];
        iyb=dists[i3+1]+1;
        dists[i3+2]=sqrt(
            (playerEdit[ixa]-playerEdit[ixb])*(playerEdit[ixa]-playerEdit[ixb])+
            (playerEdit[iya]-playerEdit[iyb])*(playerEdit[iya]-playerEdit[iyb])
        );
    }
}
void DesignerState_event(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags,SDL_Event* evt)
{
    if(evt->key.keysym.sym==SDLK_t)
        *next=&TestState;
}
void DesignerState_frame(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags)
{
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, SDL_ALPHA_OPAQUE);
    unsigned char i,i3=0,ixa,iya,ixb,iyb;
    int x,y;
    double mind=DBL_MAX,dist;
    Uint32 but;
    but=SDL_GetMouseState(&x,&y);
    for(i=0;i<20;i+=2)
    {
        dist=sqrt(
            (playerEdit[i]-x)*(playerEdit[i]-x)+
            (playerEdit[i+1]-y)*(playerEdit[i+1]-y)
        );
        if(dist<mind)
        {
            i3=i;
            mind=dist;
        }
    }
    SDL_RenderDrawLine(renderer,playerEdit[i3]-10,playerEdit[i3+1]-10,playerEdit[i3]+10,playerEdit[i3+1]+10);
    SDL_RenderDrawLine(renderer,playerEdit[i3]-10,playerEdit[i3+1]+10,playerEdit[i3]+10,playerEdit[i3+1]-10);
    if(SDL_BUTTON(1)&but)
    {
        if(!(brokenflag&01))
        {
            if(i3!=0&&i3!=8)
            {
                playerEdit[i3]=x;
                playerEdit[i3+1]=y;
            }
            else
            playerEdit[i3]=x;
        }
    }
    else if(brokenflag&01)brokenflag&=!01;

    /*Rigidbody constraint loop*/
    mind=DBL_MAX;
    double dx,dy;
    for(int ii=0;ii<3;ii++)
    {
        for(i=0;i<9;i++)
        {
            i3=3*i;
            ixa=dists[i3];
            iya=dists[i3]+1;
            ixb=dists[i3+1];
            iyb=dists[i3+1]+1;
            dx=playerEdit[ixb]-playerEdit[ixa];
            dy=playerEdit[ixb+1]-playerEdit[ixa+1];
            dist=(sqrt(dx*dx+dy*dy)-dists[i3+2])/dists[i3+2];
            if(isnan(dist)||isinf(dist))
            {
                reset_edit();
                brokenflag|=01;
                goto draw;
            }
            mind=mind>dist?dist:mind;
            if(ixa!=8&&ixa!=0)
            {
                playerEdit[ixa]+=dx*dist*.5;
                playerEdit[ixa+1]+=dy*dist*.5;
            }
            if(ixb!=8&&ixb!=0)
            {
                playerEdit[ixb]-=dx*dist*.5;
                playerEdit[ixb+1]-=dy*dist*.5;
            }
        }
    }
    draw:
    for(i=0;i<9;i++)
    {
        i3=3*i;
        ixa=dists[i3];
        iya=dists[i3]+1;
        ixb=dists[i3+1];
        iyb=dists[i3+1]+1;
        if(i==8)SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        squigglyLine(playerEdit[ixa],playerEdit[iya],playerEdit[ixb],playerEdit[iyb],5,2,renderer);
    }
}
void DesignerState_destroy()
{
}
ac_state DesignerState={
    .stateVars=&DesignerState_Vars,
    .init=DesignerState_init,
    .event=DesignerState_event,
    .frame=DesignerState_frame,
    .destroy=DesignerState_destroy,
    .id=1
};