/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/

#include "../states.h"
#include "../statelist.h"
#include "../../lib/Simple-SDL2-Audio/src/audio.h"
#include "../Audio/audiolist.h"
#include "../Logic/ll.h"
#include "../objectlist.h"
#include <stdio.h>

ll* test_world=NULL;

struct {} TestState_Vars={};
void TestState_init(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags)
{
    printf("In test state.\n");
    ll_push(&test_world,Player(100,100,0,test_world));
    /*playMusicFromMemory(deathsfolia,SDL_MIX_MAXVOLUME);*/
}
void TestState_event(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags,SDL_Event* evt)
{
    if(evt->type==SDL_KEYDOWN&&evt->key.keysym.sym==SDLK_p)
    {
        *next=&DesignerState;
        return;
    }
    ll* curObj=test_world;
    for(;curObj!=NULL;curObj=curObj->next)
        ((ac_object*)curObj->self)->event((ac_object*)curObj->self,self,next,renderer,window,ac_flags,evt);
}
void TestState_frame(struct ac_state** self,struct ac_state** next,SDL_Renderer* renderer,SDL_Window* window,char* ac_flags)
{
    ll* curObj=test_world;
    for(;curObj!=NULL;curObj=curObj->next)((ac_object*)test_world->self)->frame((ac_object*)test_world->self,self,next,renderer,window,ac_flags);
}
void TestState_destroy()
{
    stopMusic();
    ll* curObj=test_world;
    for(;curObj!=NULL;curObj=curObj->next)
    {
        ((ac_object*)test_world->self)->destroy((ac_object**)&test_world->self);
    }
    ll_free(&test_world);
}
ac_state TestState={
    .stateVars=&TestState_Vars,
    .init=TestState_init,
    .event=TestState_event,
    .frame=TestState_frame,
    .destroy=TestState_destroy,
    .id=255
};