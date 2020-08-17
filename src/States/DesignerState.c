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
#include "../sharedfonts.h"
#include "../Maths/Vect2d/Vect2d.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

double Designer_dists[]={
    0,2,0,
    2,4,0,
    4,6,0,
    6,8,0,
    4,10,0,
    10,12,0,
    10,14,0,/*head*/
    10,16,0,/*swordarm*/
    16,18,0/*sword*/
};
SDL_Colour Designer_txtcol={255,255,255};
char Designer_savemsg[26];
SDL_Surface* Designer_msg_sur=NULL;
SDL_Texture* Designer_msg_txt=NULL;
unsigned char Designer_lastsave=255;
double Designer_defaultEdit[20];
double Designer_playerEdit[20];
double designer_0[20];
double designer_1[20];
double designer_2[20];
double designer_3[20];
double designer_4[20];
double* designer_states[]={designer_0,designer_1,designer_2,designer_3,designer_4};
const double playerDesigner_defaultEdit[]={
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
SDL_Rect Designer_txtpos={0,430,0,0};
unsigned char Designer_brokenflag=0;
void reset_edit()
{
    for(char i=0;i<20;i++)
        Designer_playerEdit[i]=Designer_defaultEdit[i];
}
void DesignerState_save(char state,SDL_Renderer* renderer)
{
    double x=Designer_playerEdit[0]/10,y=Designer_playerEdit[1]/10;
    for(unsigned char i=0;i<20;i++)
        designer_states[state][i]=(Designer_playerEdit[i]/10)-(i%2==0?x:y);
    Designer_lastsave=state;
    Designer_savemsg[24]=Designer_lastsave+'0';
    if(!Designer_msg_txt)
    {
        SDL_FreeSurface(Designer_msg_sur);
        SDL_DestroyTexture(Designer_msg_txt);
    }
    Designer_msg_sur=TTF_RenderText_Solid(uni0553,Designer_savemsg,Designer_txtcol);
    Designer_msg_txt=SDL_CreateTextureFromSurface(renderer,Designer_msg_sur);
    SDL_QueryTexture(Designer_msg_txt,NULL,NULL,&Designer_txtpos.w,&Designer_txtpos.h);
}
struct {} DesignerState_Vars={};
void DesignerState_init(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags)
{
    printf("In designer state.\n");
    unsigned char i,i3,ixa,iya,ixb,iyb;
    strcpy(Designer_savemsg,"Position saved as input 0");
    Designer_lastsave=255;
    for(i=0;i<20;i++)
    {
        Designer_playerEdit[i]=playerDesigner_defaultEdit[i];
        Designer_playerEdit[i]*=10;
        if(i%2==1)
            Designer_playerEdit[i]=460-Designer_playerEdit[i];
        else Designer_playerEdit[i]+=100;
        Designer_defaultEdit[i]=Designer_playerEdit[i];
    }
    for(i=0;i<9;i++)
    {
        i3=3*i;
        ixa=Designer_dists[i3];
        iya=Designer_dists[i3]+1;
        ixb=Designer_dists[i3+1];
        iyb=Designer_dists[i3+1]+1;
        Designer_dists[i3+2]=sqrt(
            (Designer_playerEdit[ixa]-Designer_playerEdit[ixb])*(Designer_playerEdit[ixa]-Designer_playerEdit[ixb])+
            (Designer_playerEdit[iya]-Designer_playerEdit[iyb])*(Designer_playerEdit[iya]-Designer_playerEdit[iyb])
        );
    }
    for(i=0;i<5;i++)DesignerState_save(i,renderer);
    Designer_lastsave=255;
}
void DesignerState_event(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags,SDL_Event* evt)
{
    if(evt->type==SDL_KEYDOWN)
    {
        switch (evt->key.keysym.sym)
        {
        case SDLK_t:
            *next=&TestState;
            break;
        case SDLK_0:
            DesignerState_save(0,renderer);
            break;
        case SDLK_1:
            DesignerState_save(1,renderer);
            break;
        case SDLK_2:
            DesignerState_save(2,renderer);
            break;
        case SDLK_3:
            DesignerState_save(3,renderer);
            break;
        case SDLK_4:
            DesignerState_save(4,renderer);
            break;
        }
    }
}
void DesignerState_frame(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags)
{
    if(Designer_lastsave!=255)
    {
        SDL_RenderCopy(renderer,Designer_msg_txt,NULL,&Designer_txtpos);
    }
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, SDL_ALPHA_OPAQUE);
    unsigned char i,i3=0,ixa,iya,ixb,iyb;
    int x,y;
    double mind=DBL_MAX,dist;
    Uint32 but;
    but=SDL_GetMouseState(&x,&y);
    for(i=0;i<20;i+=2)
    {
        dist=sqrt(
            (Designer_playerEdit[i]-x)*(Designer_playerEdit[i]-x)+
            (Designer_playerEdit[i+1]-y)*(Designer_playerEdit[i+1]-y)
        );
        if(dist<mind)
        {
            i3=i;
            mind=dist;
        }
    }
    SDL_RenderDrawLine(renderer,Designer_playerEdit[i3]-10,Designer_playerEdit[i3+1]-10,Designer_playerEdit[i3]+10,Designer_playerEdit[i3+1]+10);
    SDL_RenderDrawLine(renderer,Designer_playerEdit[i3]-10,Designer_playerEdit[i3+1]+10,Designer_playerEdit[i3]+10,Designer_playerEdit[i3+1]-10);

    if(SDL_BUTTON(1)&but)
    {
        if(!(Designer_brokenflag&01))
        {
            if(i3!=0&&i3!=8)
            {
                Designer_playerEdit[i3]=x;
                Designer_playerEdit[i3+1]=y;
            }
            else
            Designer_playerEdit[i3]=x;
        }
    }
    else if(Designer_brokenflag&01)Designer_brokenflag&=~01;

    /*Rigidbody constraint loop*/
    mind=DBL_MAX;
    double dx,dy;
    vect2d sacva,sacvb,sacvc;
    double sqarmlengtht,projectiont,normalprojectiont;
    for(int ii=0;ii<3;ii++)
    {
        /*sword angle constraint values*/
        sacva.i=Designer_playerEdit[10];
        sacva.j=Designer_playerEdit[11];
        sacvb.i=Designer_playerEdit[16];
        sacvb.j=Designer_playerEdit[17];
        sacvc.i=Designer_playerEdit[18],
        sacvc.j=Designer_playerEdit[19];
        sub2d(&sacvb,&sacva,&sacvb);/*sacvb=arm*/
        sub2d(&sacvc,&sacva,&sacvc);/*sacvc=sword*/
        sqarmlengtht=dotProduct2d(&sacvb,&sacvb);
        projectiont=dotProduct2d(&sacvb,&sacvc)/sqarmlengtht;
        add2dscale(&sacvc,-projectiont,&sacvb,&sacva);
        normalprojectiont=sqrt(dotProduct2d(&sacva,&sacva)/sqarmlengtht);
        for(i=0;i<9;i++)
        {
            i3=3*i;
            ixa=Designer_dists[i3];
            iya=Designer_dists[i3]+1;
            ixb=Designer_dists[i3+1];
            iyb=Designer_dists[i3+1]+1;

            dx=Designer_playerEdit[ixb]-Designer_playerEdit[ixa];
            dy=Designer_playerEdit[ixb+1]-Designer_playerEdit[ixa+1];

            dist=(sqrt(dx*dx+dy*dy)-Designer_dists[i3+2])/Designer_dists[i3+2];
            if(isnan(dist)||isinf(dist))
            {
                reset_edit();
                Designer_brokenflag|=01;
                goto draw;
            }
            mind=mind>dist?dist:mind;
            if(ixa!=8&&ixa!=0)
            {
                Designer_playerEdit[ixa]+=dx*dist*.5;
                Designer_playerEdit[ixa+1]+=dy*dist*.5;
            }
            if(ixb!=8&&ixb!=0)
            {
                Designer_playerEdit[ixb]-=dx*dist*.5;
                Designer_playerEdit[ixb+1]-=dy*dist*.5;
            }

            /*Head angle constraint*/

            /*Sword angle constraint*/
            if(ixa==10)
            {
                sacva.i=Designer_playerEdit[10];
                sacva.j=Designer_playerEdit[11];
                sacvb.i=Designer_playerEdit[16];
                sacvb.j=Designer_playerEdit[17];
                sacvc.i=0;sacvc.j=0;/*Our final sword position (minus the root of the arm)*/
                sub2d(&sacvb,&sacva,&sacva);/*sacva=arm*/

                /*sacvb=perpendicular component of the sword's position*/
                sacvb.i=-sacva.j;
                sacvb.j=sacva.i;
                scale2d(&sacvb,normalprojectiont,&sacvb);

                /*
                sacvc=sacva*projectiont+sacvb
                that is, sacva gives us the component parallel to the arm
                and sacvb is the perpendicular component.
                */
                add2dscale(&sacvc,-projectiont,&sacva,&sacvc);
                add2d(&sacvc,&sacvb,&sacvc);

                /*Then we reposition the sword*/
                Designer_playerEdit[18]=Designer_playerEdit[10]+sacvc.i;
                Designer_playerEdit[19]=Designer_playerEdit[11]+sacvc.j;
            }
        }
    }
    draw:
    for(i=0;i<9;i++)
    {
        i3=3*i;
        ixa=Designer_dists[i3];
        iya=Designer_dists[i3]+1;
        ixb=Designer_dists[i3+1];
        iyb=Designer_dists[i3+1]+1;
        if(i==8)SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        squigglyLine(Designer_playerEdit[ixa],Designer_playerEdit[iya],Designer_playerEdit[ixb],Designer_playerEdit[iyb],5,2,renderer);
    }
}
void DesignerState_destroy()
{
    SDL_DestroyTexture(Designer_msg_txt);
    SDL_FreeSurface(Designer_msg_sur);
}

ac_state DesignerState={
    .stateVars=&DesignerState_Vars,
    .init=DesignerState_init,
    .event=DesignerState_event,
    .frame=DesignerState_frame,
    .destroy=DesignerState_destroy,
    .id=1
};