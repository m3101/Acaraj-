/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/

#include "../states.h"
#include "../sharedfonts.h"
#include "../Rendering/sqline.h"
#include "../statelist.h"
#include <stdlib.h>

#include "../Audio/AudioSys.h"
#include "../Audio/audiolist.h"
#include "../Audio/musicMixer.h"

#if AC_USE_TTS
#include <espeak-ng/speak_lib.h>
#endif

struct {} MenuState_Vars={};

#if AC_USE_TTS
static int SynthIntro(void* arg)
{
    espeak_SetVoiceByName("en");
    espeak_Synth("This...is......",500,0,0,0,0,NULL,NULL);
    espeak_SetVoiceByName("pt");
    espeak_Synth(".........Acarajéééé",500,0,0,0,0,NULL,NULL);
    return 0;
}
#endif 

static char selected = 0;

SDL_Colour Menu_txtcol={200,200,200};

SDL_Rect Title_pos={170,0,300,200};
SDL_Rect VTitle_pos={170,0,300,200};
SDL_Surface* Title_sur=NULL;
SDL_Texture* Title_txt=NULL;

SDL_Rect Subtitle_pos={0,210,640,30};
SDL_Surface* Subtitle_sur=NULL;
SDL_Texture* Subtitle_txt=NULL;

SDL_Rect Btn1_pos={240,240,160,60};
SDL_Surface* Btn1_sur=NULL;
SDL_Texture* Btn1_txt=NULL;

SDL_Rect Btn2_pos={240,340,160,60};
SDL_Surface* Btn2_sur=NULL;
SDL_Texture* Btn2_txt=NULL;

static char dintro = 0;

char btn2=0;

void MenuState_init(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags)
{
    if(!dintro)
    {
        dintro = 1;
        #if AC_USE_TTS
        SDL_CreateThread(SynthIntro,"Synthetiser",NULL);
        #endif
        
    }

    Title_sur=TTF_RenderText_Solid(uni0553,"ACARAJE",Menu_txtcol);
    Title_txt=SDL_CreateTextureFromSurface(renderer,Title_sur);

    Subtitle_sur=TTF_RenderText_Solid(uni0553,"Version 0.0.0 Alpha - by Amelia 0. F. da S.",Menu_txtcol);
    Subtitle_txt=SDL_CreateTextureFromSurface(renderer,Subtitle_sur);

    Btn1_sur=TTF_RenderText_Solid(uni0553,"MOVESET DESIGNER",Menu_txtcol);
    Btn1_txt=SDL_CreateTextureFromSurface(renderer,Btn1_sur);

    Btn2_sur=TTF_RenderText_Solid(uni0553,"EN GARDE!",Menu_txtcol);
    Btn2_txt=SDL_CreateTextureFromSurface(renderer,Btn2_sur);
}
void MenuState_event(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags,SDL_Event* evt)
{
    if(evt->type==SDL_KEYDOWN)
    {
        switch (evt->key.keysym.sym)
        {
            case SDLK_DOWN:
            case SDLK_UP:
                btn2 = !btn2;
            break;
            case SDLK_e:
                if(btn2)
                {
                    *next=&TestState;
                }
                else *next=&DesignerState;
            break;
        }
    }
}
void MenuState_frame(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags)
{
    playAutomusic(&mainSong);
    float r = (float)rand()/RAND_MAX;
    VTitle_pos.x=Title_pos.x+((int)(r*5));
    r = (float)rand()/RAND_MAX;
    VTitle_pos.y=Title_pos.y+((int)(r*5));
    SDL_RenderCopy(renderer,Title_txt,NULL,&VTitle_pos);

    SDL_RenderCopy(renderer,Subtitle_txt,NULL,&Subtitle_pos);
    SDL_RenderCopy(renderer,Btn1_txt,NULL,&Btn1_pos);
    SDL_RenderCopy(renderer,Btn2_txt,NULL,&Btn2_pos);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, SDL_ALPHA_OPAQUE);
    double x0,x1,y0,y1;
    x0=VTitle_pos.x-r;
    y0=VTitle_pos.y+VTitle_pos.h-20;
    x1=VTitle_pos.x+VTitle_pos.w+r;
    y1=y0;
    squigglyLine(x0,y0,x1,y1,10,10,renderer);
    if(!btn2)
    {
        x0=Btn1_pos.x-20;
        y0=Btn1_pos.y+Btn1_pos.h;
        x1=Btn1_pos.x+Btn1_pos.w+20;
        y1=y0;
    }
    else
    {
        x0=Btn2_pos.x-20;
        y0=Btn2_pos.y+Btn2_pos.h;
        x1=Btn2_pos.x+Btn2_pos.w+20;
        y1=y0;
    }
    squigglyLine(x0,y0,x1,y1,5,20,renderer);
}
void MenuState_destroy()
{
    SDL_DestroyTexture(Title_txt);
    SDL_FreeSurface(Title_sur);

    SDL_DestroyTexture(Subtitle_txt);
    SDL_FreeSurface(Subtitle_sur);

    SDL_DestroyTexture(Btn1_txt);
    SDL_FreeSurface(Btn1_sur);

    SDL_DestroyTexture(Btn2_txt);
    SDL_FreeSurface(Btn2_sur);
}
ac_state MenuState={
    .stateVars=&MenuState_Vars,
    .init=MenuState_init,
    .event=MenuState_event,
    .frame=MenuState_frame,
    .destroy=MenuState_destroy,
    .id=2
};